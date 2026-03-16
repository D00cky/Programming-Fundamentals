# 05 — CI/CD & DevOps

> *"If it hurts, do it more often."* — Jez Humble

## Antes de começar

Certifique-se de que você já:

- [ ] Usa git com branches, commits e pull requests fluentemente (`fase0-fundacao`)
- [ ] Tem Docker instalado e rodou pelo menos um `docker run` de uma imagem existente
- [ ] Concluiu `fase6-eng-software/03-testes` — CI sem testes é pouco útil

---

## O que você vai aprender

Ao final deste módulo você será capaz de:

- Configurar um workflow completo no GitHub Actions (build, test, docker build, push)
- Escrever um Dockerfile multi-stage eficiente para uma aplicação Java
- Subir um ambiente de desenvolvimento com docker-compose (app + banco + cache)
- Explicar os conceitos de CI, CD, Continuous Deployment e as diferenças entre eles
- Interpretar métricas de observabilidade: logs estruturados, métricas, distributed traces

---

## 1. Conceitos

**Continuous Integration (CI):** integrar código frequentemente (vários commits por dia), cada integração verifica automaticamente com build + testes.

**Continuous Delivery (CD):** manter o código sempre em estado deployável. Deploy requer aprovação manual.

**Continuous Deployment:** deploy automático a cada commit que passa nos testes.

**Por que importa:** detectar bugs cedo (quando são baratos) em vez de tarde (quando são caros e vergonhosos).

---

## 2. GitHub Actions

```yaml
# .github/workflows/ci.yml
name: CI

on:
  push:
    branches: [main, develop]
  pull_request:
    branches: [main]

jobs:
  build-and-test:
    runs-on: ubuntu-latest

    steps:
      - uses: actions/checkout@v4

      - name: Set up JDK 21
        uses: actions/setup-java@v4
        with:
          java-version: '21'
          distribution: 'temurin'

      - name: Build
        run: mvn compile

      - name: Test
        run: mvn test

      - name: Code Coverage
        run: mvn jacoco:report

      - name: Upload Coverage Report
        uses: codecov/codecov-action@v3

  # Job para C:
  build-c:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Compile
        run: gcc -Wall -Wextra -Werror main.c -o programa
      - name: Valgrind
        run: valgrind --error-exitcode=1 --leak-check=full ./programa
```

---

## 3. Docker

Empacotar aplicação com todas as dependências em um container isolado.

```dockerfile
# Dockerfile para aplicação Java
FROM eclipse-temurin:21-jdk-alpine AS builder
WORKDIR /app
COPY pom.xml .
COPY src ./src
RUN mvn package -DskipTests

FROM eclipse-temurin:21-jre-alpine
WORKDIR /app
COPY --from=builder /app/target/*.jar app.jar
EXPOSE 8080
ENTRYPOINT ["java", "-jar", "app.jar"]
```

```bash
# Construir imagem:
docker build -t minha-app:1.0 .

# Rodar container:
docker run -p 8080:8080 minha-app:1.0

# Com variáveis de ambiente:
docker run -e DATABASE_URL=jdbc:postgresql://... -p 8080:8080 minha-app:1.0

# Listar containers:
docker ps
docker ps -a   # incluindo parados
docker logs <container_id>
docker exec -it <container_id> sh
```

---

## 4. Docker Compose

Orquestrar múltiplos containers localmente:

```yaml
# docker-compose.yml
version: '3.8'

services:
  app:
    build: .
    ports:
      - "8080:8080"
    environment:
      - DATABASE_URL=jdbc:postgresql://db:5432/myapp
      - DATABASE_USER=myuser
      - DATABASE_PASSWORD=mypass
    depends_on:
      - db

  db:
    image: postgres:16-alpine
    environment:
      POSTGRES_DB: myapp
      POSTGRES_USER: myuser
      POSTGRES_PASSWORD: mypass
    volumes:
      - postgres_data:/var/lib/postgresql/data
    ports:
      - "5432:5432"

  redis:
    image: redis:7-alpine
    ports:
      - "6379:6379"

volumes:
  postgres_data:
```

```bash
docker compose up -d       # subir em background
docker compose logs -f app # ver logs da aplicação
docker compose down        # parar e remover containers
docker compose down -v     # também remove volumes
```

---

## 5. Pipeline Completo

```
Developer Push
     ↓
[GitHub Actions CI]
     ├── Checkout
     ├── Build
     ├── Unit Tests
     ├── Integration Tests
     ├── Code Coverage Check (≥ 80%)
     ├── Static Analysis (Checkstyle, SpotBugs)
     └── Security Scan (OWASP)
     ↓ (se passou)
[Build Docker Image]
     ↓
[Push to Registry] (Docker Hub, ECR, GCR)
     ↓
[Deploy to Staging] (automático)
     ↓
[Smoke Tests]
     ↓
[Deploy to Production] (manual approval ou automático)
     ↓
[Health Check]
     ↓
[Notify Slack]
```

---

## 6. Observabilidade — Logs, Métricas, Traces

```java
// Structured logging (JSON) — fácil de indexar:
logger.info("Pedido processado",
    "pedidoId", pedido.getId(),
    "valor", pedido.getTotal(),
    "duracao_ms", duration);

// Métricas com Micrometer (Spring Boot):
Counter.builder("pedidos.processados")
    .tag("status", "sucesso")
    .register(meterRegistry)
    .increment();
```

**Stack de observabilidade:**
- **Logs:** ELK Stack (Elasticsearch + Logstash + Kibana) ou Loki + Grafana
- **Métricas:** Prometheus + Grafana
- **Traces:** Jaeger ou Zipkin (distributed tracing)

---

## 7. Conceitos de SRE

- **SLA:** Service Level Agreement — contrato de disponibilidade (ex: 99.9%)
- **SLO:** Service Level Objective — meta interna (ex: 99.95%)
- **Error Budget:** quanto downtime é aceitável por período
- **MTTR:** Mean Time To Recovery — tempo médio para recuperar de incidente
- **MTBF:** Mean Time Between Failures — tempo médio entre falhas
- **On-call:** rotação de plantão para responder alertas em produção

---

## Exercícios

**ex01:** configurar GitHub Actions que roda testes a cada push; falhar se cobertura < 70%

**ex02:** dockerizar a aplicação Java com multi-stage build; adicionar docker-compose com PostgreSQL

**ex03:** implementar health check endpoint (`/actuator/health`); configurar alerta se endpoint ficar down por > 1 minuto

---

## Knowledge Check

Responda sem consultar o material. Se travar, releia a seção correspondente.

1. Qual a diferença entre Continuous Delivery e Continuous Deployment?
2. O que é um multi-stage Dockerfile? Qual benefício ele traz em relação ao tamanho da imagem?
3. Por que `docker-compose` é útil em desenvolvimento mas não é adequado para produção em escala?
4. O que acontece se um step do GitHub Actions falha? Como tornar um step opcional?
5. Qual a diferença entre `ENV` e `ARG` em um Dockerfile?
6. O que é um health check em Docker? Como ele é útil no `docker-compose`?
7. O que é observabilidade? Qual a diferença entre logs, métricas e distributed traces?
8. O que é Error Budget no contexto de SRE? Como ele conecta desenvolvimento e operações?

---

## Projeto — Pipeline CI/CD Completo para Aplicação Java

Configure um pipeline GitHub Actions completo para um projeto Java com Maven:

**Estrutura do projeto alvo:**
- Aplicação Spring Boot simples com endpoint REST `/health` e `/api/items`
- JUnit 5 com pelo menos 5 testes
- JaCoCo para cobertura

**Passo 1 — GitHub Actions (`.github/workflows/ci.yml`):**
```yaml
on: [push, pull_request]
jobs:
  ci:
    steps:
      - checkout
      - setup JDK 21
      - mvn compile
      - mvn test
      - jacoco:report
      - falhar se cobertura < 70%
      - checkstyle
  docker:
    needs: ci
    steps:
      - docker build
      - docker run (smoke test: curl /health)
      - docker push (apenas em push para main)
```

**Passo 2 — Dockerfile multi-stage:**
- Stage `builder`: JDK + Maven, compila e empacota
- Stage `runtime`: JRE-alpine apenas, copia o `.jar`
- Imagem final < 200MB
- `HEALTHCHECK` configurado

**Passo 3 — docker-compose.yml:**
- Serviço `app` (sua imagem)
- Serviço `db` (PostgreSQL)
- Variáveis de ambiente via `.env` (nunca hardcoded)
- `depends_on` com `condition: service_healthy`

**Critérios de aceite:**
- `git push` na branch feature → CI roda e exibe ✅ ou ❌
- `git push` em main → imagem publicada no GitHub Container Registry
- `docker compose up` sobe o ambiente completo em < 30s

---

## Referências

- **Continuous Delivery** — Jez Humble & David Farley
- **The Phoenix Project** — Gene Kim · romance sobre DevOps (leitura muito recomendada)
- **Site Reliability Engineering** — Google · sre.google/books (gratuito online)
- **Docker Documentation** — docs.docker.com
- **GitHub Actions Documentation** — docs.github.com/actions

---

## Recursos Adicionais

Estes recursos são **opcionais** mas vão solidificar seu entendimento:

**Para ler/assistir agora:**
- **The Phoenix Project** — Gene Kim · romance sobre DevOps; leitura leve que constrói intuição sobre por que CI/CD importa antes de qualquer ferramenta
- **Docker documentation** (docs.docker.com) — tutorial oficial Getting Started; cobre 80% do que você precisará no dia a dia

**Para consulta:**
- **GitHub Actions documentation** — referência completa de syntax, contexts, expressions e marketplace de actions
- **Docker Compose reference** — spec completa do `docker-compose.yml` com todos os campos

**Para ir além:**
- **Continuous Delivery** — Humble & Farley · o livro que definiu a disciplina; capítulos 5-8 são os mais práticos
- **Site Reliability Engineering** — Google (sre.google/books, gratuito) · como Google opera sistemas em escala com SLOs, error budgets e on-call
