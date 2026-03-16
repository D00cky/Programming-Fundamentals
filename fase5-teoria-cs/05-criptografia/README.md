# 05 — Criptografia

> *"Security without cryptography is an oxymoron."*

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

## Referências

- **Cryptography and Network Security** — Stallings
- **Applied Cryptography** — Bruce Schneier
- **Cryptopals Challenges** — cryptopals.com · praticar criptografia atacando implementações
- **crypto101.io** — introdução visual e gratuita
- **libsodium** — biblioteca criptográfica moderna, use em produção
