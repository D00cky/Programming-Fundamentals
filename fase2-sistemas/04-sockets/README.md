# 04 — Sockets & Redes

> *"The network is the computer."* — John Gage

---

## Antes de começar

Certifique-se de que você já:

- [ ] Implementou `fork()`, `exec()` e `waitpid()` e entende processos filhos
- [ ] Usou `dup2()` e pipes para redirecionar I/O
- [ ] Criou threads com pthreads e usou mutex/semáforos (`fase2-sistemas/03`)
- [ ] Conhece os conceitos de `select()` ou `poll()` em nível conceitual

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Implementar um servidor e cliente TCP do zero com as syscalls de socket
- Suportar múltiplos clientes simultâneos com `fork()` ou `select()`/`poll()`
- Explicar o modelo cliente-servidor e as camadas da pilha OSI relevantes (L3/L4)
- Testar servidores com `nc` (netcat) e inspecionar tráfego com Wireshark
- Usar `htons`/`ntohs` corretamente para conversão de byte order

---

## 1. Modelo Cliente-Servidor

```
Cliente                    Servidor
  |                           |
  |──── connect() ──────────→ |  (TCP handshake)
  |                           |
  |──── send("GET /") ──────→ |
  |                           |
  |←─── recv("200 OK...") ── |
  |                           |
  |──── close() ────────────→ |
```

---

## 2. Socket TCP — Servidor

```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void) {
    // 1. Criar socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);

    // 2. Reusar porta (evita "Address already in use")
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. Bind: associar a porta 8080
    struct sockaddr_in addr = {0};
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  // aceita de qualquer IP
    addr.sin_port        = htons(8080); // htons: host to network byte order
    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));

    // 4. Listen: fila de conexões pendentes
    listen(server_fd, 10);
    printf("Servidor ouvindo na porta 8080...\n");

    // 5. Accept: bloqueia até cliente conectar
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd,
                               (struct sockaddr *)&client_addr,
                               &client_len);

        printf("Cliente conectado: %s\n", inet_ntoa(client_addr.sin_addr));

        // 6. Comunicar
        char buf[1024];
        int n = recv(client_fd, buf, sizeof(buf) - 1, 0);
        buf[n] = '\0';
        printf("Recebido: %s\n", buf);
        send(client_fd, "Olá, cliente!\n", 14, 0);

        close(client_fd);
    }
    close(server_fd);
    return 0;
}
```

## 3. Socket TCP — Cliente

```c
int main(void) {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server = {0};
    server.sin_family = AF_INET;
    server.sin_port   = htons(8080);
    inet_pton(AF_INET, "127.0.0.1", &server.sin_addr);

    connect(sockfd, (struct sockaddr *)&server, sizeof(server));

    send(sockfd, "Olá, servidor!", 14, 0);

    char buf[1024];
    int n = recv(sockfd, buf, sizeof(buf) - 1, 0);
    buf[n] = '\0';
    printf("Servidor disse: %s\n", buf);

    close(sockfd);
    return 0;
}
```

---

## 4. Múltiplos Clientes

**Abordagem 1: fork por cliente**
```c
int client_fd = accept(...);
if (fork() == 0) {
    close(server_fd);
    handle_client(client_fd);
    exit(0);
}
close(client_fd);
```

**Abordagem 2: select() — I/O multiplexing**
```c
fd_set fds;
FD_ZERO(&fds);
FD_SET(server_fd, &fds);
// Adicionar client_fds...

select(max_fd + 1, &fds, NULL, NULL, NULL);

if (FD_ISSET(server_fd, &fds))
    // Nova conexão
if (FD_ISSET(client_fd, &fds))
    // Dados do cliente
```

---

## 5. Byte Order

Redes usam *big-endian* (network byte order). x86 usa *little-endian*.

```c
// Conversão obrigatória para portas e IPs:
htons(8080)           // host → network short (porta)
htonl(addr)           // host → network long (IP)
ntohs(port)           // network → host short
ntohl(addr)           // network → host long

// Converter string IP:
inet_pton(AF_INET, "192.168.1.1", &addr.sin_addr);
inet_ntoa(addr.sin_addr)  // IP → string (não thread-safe)
```

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre TCP e UDP? Quando você usaria cada um?
2. Qual a diferença entre `bind()` e `connect()`? Quem chama cada um?
3. O que o parâmetro `backlog` de `listen()` representa?
4. Por que é obrigatório usar `htons()` para a porta, mesmo que o valor seja pequeno?
5. Qual a diferença entre usar `send()` e `write()` em um socket?
6. Qual a diferença entre `select()`, `poll()` e `epoll()`?
7. O que é o estado TIME_WAIT do TCP? Por que `SO_REUSEADDR` é necessário?
8. Por que `SO_REUSEADDR` pode ser perigoso se não entendido corretamente?

---

## Projeto — Servidor de Chat Multi-cliente

Implemente um servidor de chat TCP onde múltiplos clientes podem enviar mensagens que são retransmitidas para todos os outros.

**Funcionalidades:**
- Aceitar N clientes simultâneos (sem limite fixo)
- Broadcast de mensagens: quando um cliente envia, todos recebem
- Mensagem de entrada/saída ao conectar/desconectar
- Usar `select()` para multiplexar (sem thread por cliente)

**Requisitos técnicos:**
- `SO_REUSEADDR` configurado no servidor
- Compilar com `gcc -Wall -Wextra -Werror`
- Testar com 3 terminais usando `nc localhost 8080`
- Tratar desconexão abrupta sem crash

**Exemplo de execução (3 terminais):**
```
# Terminal 1: servidor
$ ./chat_server 8080
[+] Alice conectou
[+] Bruno conectou
[Alice] olá pessoal!
[-] Bruno desconectou

# Terminal 2: nc localhost 8080
[+] Bruno conectou
[Alice] olá pessoal!

# Terminal 3: nc localhost 8080
olá pessoal!
[-] Bruno desconectou
```

---

## Exercícios

**ex01-tcp-echo:** servidor eco que devolve tudo que recebe; testar com `nc localhost 8080`

**ex02-http-minimo:** servidor HTTP que responde `200 OK` com uma página HTML simples

**ex03-chat:** servidor de chat simples: broadcast de mensagens para todos os clientes conectados (com `select()` ou threads)

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- [Beej's Guide to Network Programming](https://beej.us/guide/bgnet/) — o melhor recurso gratuito sobre sockets em C
- **OSTEP** não cobre redes, mas os conceitos de I/O do módulo 36 ajudam

**Para consulta:**
- `man 2 socket` · `man 2 bind` · `man 2 listen` · `man 2 accept` · `man 2 connect`
- **TLPI** capítulos 56-61 (sockets) — referência completa

**Para ir além:**
- [Wireshark](https://www.wireshark.org) — capturar e inspecionar pacotes TCP em tempo real
- [The C10K Problem](http://www.kegel.com/c10k.html) — artigo clássico sobre servidores de alta performance com epoll
