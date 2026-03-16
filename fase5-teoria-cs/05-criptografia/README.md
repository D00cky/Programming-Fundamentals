# 05 — Criptografia

> *"Security without cryptography is an oxymoron."*

## Antes de começar

Certifique-se de que você já:

- [ ] Entende aritmética modular básica (resto da divisão, propriedades)
- [ ] Sabe usar o terminal Linux e instalar pacotes (`fase0-fundacao`)
- [ ] Concluiu `fase5-teoria-cs/01-complexidade` — os conceitos de P/NP são relevantes para entender por que RSA é seguro

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Diferenciar hash, MAC e assinatura digital e quando usar cada um
- Explicar por que AES-GCM é o modo recomendado e por que ECB é inseguro
- Descrever o mecanismo do Diffie-Hellman Key Exchange e por que funciona sem canal seguro
- Usar libsodium para cifrar e decifrar dados com segurança em C
- Identificar práticas criptográficas inseguras em código real

---

## 1. Fundamentos

### Terminologia
- **Plaintext:** dados originais
- **Ciphertext:** dados cifrados
- **Chave (key):** parâmetro secreto
- **Cifração:** plaintext + chave → ciphertext
- **Decifração:** ciphertext + chave → plaintext
- **Cifra simétrica:** mesma chave para cifrar e decifrar
- **Cifra assimétrica:** par de chaves (pública + privada)

### Princípio de Kerckhoffs
O sistema deve ser seguro mesmo que tudo, exceto a chave, seja de conhecimento público. Segurança por obscuridade não é segurança.

---

## 2. Funções Hash Criptográficas

Mapeiam dados de tamanho arbitrário para digest de tamanho fixo.

**Propriedades:**
- **Determinística:** mesma entrada → mesma saída
- **Resistente a pré-imagem:** dado H(x), impossível achar x
- **Resistente a segunda pré-imagem:** dado x, impossível achar y ≠ x com H(y) = H(x)
- **Resistente a colisão:** impossível achar x, y com H(x) = H(y)
- **Avalanche:** pequena mudança na entrada → completamente diferente na saída

**Algoritmos:**
- **MD5, SHA-1** — quebrados para colisões, NÃO usar em segurança
- **SHA-256, SHA-3** — seguros para uso geral
- **bcrypt, Argon2** — para senhas (lentos intencionalmente, com salt)

```bash
echo "hello" | sha256sum   # Linux
```

**Usos:** integridade de arquivos, assinaturas digitais, senhas (com salt), MACs.

---

## 3. Criptografia Simétrica

### AES (Advanced Encryption Standard)

Cifra de bloco: opera em blocos de 128 bits com chave de 128, 192 ou 256 bits.

**Modos de operação:**
- **ECB:** cada bloco cifrado independentemente — inseguro (padrões visíveis)
- **CBC:** cada bloco XOR com o anterior — precisa de IV aleatório
- **CTR:** gera keystream → XOR com plaintext — paralelizável
- **GCM:** CTR + autenticação — recomendado para uso geral

```
Cifra: Plaintext XOR IV → AES_ECB → XOR Ciphertext anterior → ...
```

**Problema da criptografia simétrica:** como compartilhar a chave secreta?

---

## 4. Criptografia Assimétrica

### RSA

Baseado na dificuldade de fatorar números grandes.

```
Geração de chaves:
1. Escolher primos grandes p, q
2. n = p × q (módulo público)
3. φ(n) = (p-1)(q-1)
4. Escolher e: mdc(e, φ(n)) = 1 (expoente público)
5. Calcular d: e×d ≡ 1 (mod φ(n)) (expoente privado)

Chave pública: (n, e)
Chave privada: (n, d)

Cifrar:   c = m^e mod n
Decifrar: m = c^d mod n

Assinar:  sig = hash(m)^d mod n
Verificar: hash(m) == sig^e mod n
```

### Diffie-Hellman Key Exchange

Permite duas partes estabelecerem chave secreta sobre canal público:

```
Alice e Bob concordam em g, p (públicos)
Alice: a (privado), envia A = g^a mod p
Bob:   b (privado), envia B = g^b mod p
Alice: K = B^a mod p = g^(ab) mod p
Bob:   K = A^b mod p = g^(ab) mod p
→ Ambos chegaram na mesma chave K sem revelá-la
```

### Curvas Elípticas (ECC)

Criptografia de chaves menores com segurança equivalente ao RSA:
- RSA 3072 bits ≈ ECC 256 bits em segurança
- Mais eficiente para dispositivos com recursos limitados
- ECDSA (assinaturas), ECDH (troca de chaves)

---

## 5. Protocolo TLS/HTTPS

```
1. Client Hello: versões suportadas, cipher suites, nonce
2. Server Hello: versão escolhida, cipher suite, nonce, certificado
3. Verificação do certificado: cadeia de confiança até CA raiz
4. Key Exchange: ECDH para estabelecer chave simétrica de sessão
5. Change Cipher Spec: comunicação passa a ser cifrada (AES-GCM)
6. Handshake finalizado: ambos confirmam integridade do handshake
7. Dados cifrados com chave de sessão simétrica
```

**Certificados X.509:** assinados por Certificate Authorities (CA). O browser tem lista de CAs confiáveis.

---

## 6. MACs e Assinaturas Digitais

**MAC (Message Authentication Code):** garante integridade e autenticidade com chave simétrica.
```
HMAC-SHA256(chave, mensagem) → tag
```

**Assinatura Digital:** garante integridade, autenticidade e não-repúdio com chave assimétrica.
```
Assinar:   sig = RSA_sign(chave_privada, SHA256(mensagem))
Verificar: SHA256(mensagem) == RSA_verify(chave_pública, sig)
```

---

## 7. O que Nunca Fazer

- Implementar sua própria criptografia (use libsodium, OpenSSL)
- Usar MD5/SHA-1 para senhas
- Armazenar senhas em plaintext ou com hash simples (use bcrypt/Argon2)
- Reusar IV/nonce em AES-CTR ou AES-GCM
- Usar ECB mode
- Usar RSA < 2048 bits

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Quais três propriedades uma função hash criptográfica deve ter? Por que cada uma importa?
2. Por que MD5 e SHA-1 são inadequados para senhas, mesmo que não estejam "quebrados" para toda finalidade?
3. Por que o modo ECB do AES é inseguro? O que acontece ao cifrar uma imagem com ECB?
4. Qual problema a criptografia assimétrica resolve que a simétrica não consegue?
5. Explique o protocolo Diffie-Hellman em palavras simples. Qual informação permanece secreta?
6. O que é um MAC? Qual a diferença entre HMAC e uma assinatura digital RSA?
7. No TLS handshake, em que momento a comunicação passa a ser cifrada simetricamente?
8. Por que reusar um nonce em AES-GCM é catastrófico para a segurança?

---

## Projeto — Ferramenta de Cifração de Arquivos

Implemente uma ferramenta de linha de comando para cifrar e decifrar arquivos usando AES-256-GCM via libsodium:

**Funcionalidades:**
- `./cryptool encrypt <arquivo> <saída>` — cifra o arquivo com senha digitada pelo usuário
- `./cryptool decrypt <arquivo_cifrado> <saída>` — decifra com a mesma senha
- A senha é derivada para chave usando `crypto_pwhash` (Argon2id) com salt aleatório
- O nonce é gerado aleatoriamente a cada cifragem e salvo no cabeçalho do arquivo cifrado
- Verificar integridade: se o arquivo foi adulterado, exibir erro e não gerar saída

**Formato do arquivo cifrado:**
```
[4 bytes: "CRPT"]  ← magic number
[32 bytes: salt]   ← para derivação de chave
[24 bytes: nonce]  ← para AES-GCM
[N bytes: ciphertext + 16 bytes tag]
```

**Exemplo de execução:**
```
$ ./cryptool encrypt segredo.txt segredo.enc
Senha: ****
Arquivo cifrado: segredo.enc (156 bytes)

$ ./cryptool decrypt segredo.enc recuperado.txt
Senha: ****
Arquivo decifrado: recuperado.txt

$ echo "adulterado" >> segredo.enc
$ ./cryptool decrypt segredo.enc output.txt
Erro: integridade comprometida — arquivo adulterado ou senha incorreta.
```

**Requisitos técnicos:**
- Em C, compilar com `gcc -Wall -Wextra -Werror -lsodium`
- Nunca exibir a senha no terminal (`getpass()` ou equivalente)
- Zerar a senha da memória após uso (`sodium_memzero`)

---

## Referências

- **Cryptography and Network Security** — Stallings
- **Applied Cryptography** — Bruce Schneier
- **Cryptopals Challenges** — cryptopals.com · praticar criptografia atacando implementações
- **crypto101.io** — introdução visual e gratuita
- **libsodium** — biblioteca criptográfica moderna, use em produção

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **crypto101.io** — introdução gratuita, visual e acessível; leitura obrigatória antes de escrever qualquer código criptográfico
- **Cryptopals Challenges** (cryptopals.com) — aprender criptografia atacando implementações reais; os melhores exercícios práticos do campo

**Para consulta:**
- **libsodium documentation** (doc.libsodium.org) — referência completa da API; use sempre que implementar algo criptográfico
- **OWASP Cryptographic Storage Cheat Sheet** — boas práticas para armazenar dados sensíveis

**Para ir além:**
- **Applied Cryptography** — Bruce Schneier · cobertura ampla de protocolos e ataques reais
- **Introduction to Modern Cryptography** — Katz & Lindell · tratamento formal com provas de segurança
