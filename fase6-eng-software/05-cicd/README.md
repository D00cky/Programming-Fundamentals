# 05 — CI/CD & DevOps

> *"If it hurts, do it more often."* — Jez Humble

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

## Referências

- **Continuous Delivery** — Jez Humble & David Farley
- **The Phoenix Project** — Gene Kim · romance sobre DevOps (leitura muito recomendada)
- **Site Reliability Engineering** — Google · sre.google/books (gratuito online)
- **Docker Documentation** — docs.docker.com
- **GitHub Actions Documentation** — docs.github.com/actions
