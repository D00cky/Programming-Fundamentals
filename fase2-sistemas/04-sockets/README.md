# 04 — Sockets & Redes

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

## Exercícios

**ex01-tcp-echo:** servidor eco que devolve tudo que recebe; testar com `nc localhost 8080`

**ex02-http-minimo:** servidor HTTP que responde `200 OK` com uma página HTML simples

**ex03-chat:** servidor de chat simples: broadcast de mensagens para todos os clientes conectados (com `select()` ou threads)

---

## Referências

- `man 2 socket` · `man 2 bind` · `man 2 listen` · `man 2 accept` · `man 2 connect`
- **Beej's Guide to Network Programming** — beej.us/guide/bgnet (o melhor recurso gratuito)
- **TLPI** capítulos 56-61 (sockets)
