# Deployment Guide

## Prerequisites

- Crystal 1.7.2+
- PostgreSQL 14+
- Lucky Framework

## Environment Variables

### Required for Production

```bash
# Database
DATABASE_URL=postgres://username:password@hostname:5432/database_name

# Server
SECRET_KEY_BASE=your_secret_key_here  # Generate with: lucky gen.secret_key
PORT=3000

# Optional
DB_HOST=localhost
DB_PORT=5432
DB_USERNAME=postgres
DB_PASSWORD=postgres
```

## Development Setup

### 1. Install Dependencies

```bash
# Install Crystal dependencies
$ shards install

# Verify installation
$ crystal --version
```

### 2. Setup Database

```bash
# Create and migrate database
$ lucky db.create
$ lucky db.migrate

# Verify migration status
$ lucky db.migrate.status
```

### 3. Run Development Server

```bash
# Start the development server with auto-reload
$ lucky dev

# Or use the Procfile.dev
$ overmind start -f Procfile.dev
```

The server will start at `http://localhost:3000`

## Production Deployment

### 1. Build the Application

```bash
# Compile for production
$ crystal build src/paphos.cr --release --no-debug

# Or use shards
$ shards build --production --release
```

### 2. Set Environment Variables

Create a `.env` file or set environment variables:

```bash
export DATABASE_URL="postgres://user:pass@host:5432/paphos_production"
export SECRET_KEY_BASE="$(lucky gen.secret_key)"
export PORT=3000
export LUCKY_ENV=production
```

### 3. Run Migrations

```bash
$ LUCKY_ENV=production ./paphos db.migrate
```

### 4. Start the Server

```bash
$ LUCKY_ENV=production ./paphos
```

## Docker Deployment

### Development with Docker Compose

```bash
# Build and start services
$ docker-compose up --build

# Run in background
$ docker-compose up -d

# View logs
$ docker-compose logs -f lucky

# Stop services
$ docker-compose down
```

### Production Docker Build

Create a `Dockerfile` for production:

```dockerfile
FROM crystallang/crystal:1.7.2-alpine

WORKDIR /app

# Install dependencies
COPY shard.yml shard.lock ./
RUN shards install --production

# Copy application
COPY . .

# Build
RUN crystal build src/paphos.cr --release --no-debug -o paphos

# Expose port
EXPOSE 3000

# Start server
CMD ["./paphos"]
```

Build and run:

```bash
$ docker build -t paphos-backend .
$ docker run -p 3000:3000 \
  -e DATABASE_URL="postgres://..." \
  -e SECRET_KEY_BASE="..." \
  paphos-backend
```

## Database Migrations

### Create a New Migration

```bash
$ lucky gen.migration CreateTableName
```

### Run Migrations

```bash
# Development
$ lucky db.migrate

# Production
$ LUCKY_ENV=production lucky db.migrate
```

### Rollback Last Migration

```bash
$ lucky db.rollback
```

### Check Migration Status

```bash
$ lucky db.migrate.status
```

## Systemd Service (Linux)

Create `/etc/systemd/system/paphos.service`:

```ini
[Unit]
Description=Paphos Backend API
After=network.target postgresql.service

[Service]
Type=simple
User=paphos
WorkingDirectory=/opt/paphos
Environment="DATABASE_URL=postgres://user:pass@localhost/paphos_production"
Environment="SECRET_KEY_BASE=your_secret_key"
Environment="PORT=3000"
Environment="LUCKY_ENV=production"
ExecStart=/opt/paphos/paphos
Restart=always
RestartSec=10

[Install]
WantedBy=multi-user.target
```

Enable and start:

```bash
$ sudo systemctl enable paphos
$ sudo systemctl start paphos
$ sudo systemctl status paphos
```

## Nginx Reverse Proxy

Example Nginx configuration:

```nginx
upstream paphos {
    server 127.0.0.1:3000;
}

server {
    listen 80;
    server_name api.yourdomain.com;

    # Redirect to HTTPS
    return 301 https://$server_name$request_uri;
}

server {
    listen 443 ssl http2;
    server_name api.yourdomain.com;

    ssl_certificate /path/to/cert.pem;
    ssl_certificate_key /path/to/key.pem;

    location / {
        proxy_pass http://paphos;
        proxy_set_header Host $host;
        proxy_set_header X-Real-IP $remote_addr;
        proxy_set_header X-Forwarded-For $proxy_add_x_forwarded_for;
        proxy_set_header X-Forwarded-Proto $scheme;
    }
}
```

## Health Check Endpoint

The root endpoint `/` can be used for health checks:

```bash
$ curl http://localhost:3000/api/v1/
{"hello":"Hello World from Home::Index"}
```

## Monitoring

### Application Logs

Lucky uses Dexter for logging. Logs are written to STDOUT.

In production, redirect to a file or use a log aggregation service:

```bash
$ ./paphos >> /var/log/paphos/app.log 2>&1
```

### Database Connection Pool

Monitor PostgreSQL connections:

```sql
SELECT * FROM pg_stat_activity WHERE datname = 'paphos_production';
```

## Performance Tuning

### Database Indexes

The application includes indexes on:
- `characters.slug` (unique)
- `chat_participants(chat_id, character_id)` (unique)
- `messages(chat_id, created_at)`

### Connection Pooling

Avram uses connection pooling by default. Configure in `config/database.cr`:

```crystal
AppDatabase.configure do |settings|
  settings.credentials = Avram::Credentials.parse(ENV["DATABASE_URL"])
  # Add pool size configuration if needed
end
```

## Backup Strategy

### Database Backups

```bash
# Backup
$ pg_dump -h localhost -U postgres paphos_production > backup.sql

# Restore
$ psql -h localhost -U postgres paphos_production < backup.sql
```

### Automated Backups

Set up a cron job:

```bash
0 2 * * * /usr/bin/pg_dump -h localhost -U postgres paphos_production | gzip > /backups/paphos_$(date +\%Y\%m\%d).sql.gz
```

## Security Checklist

- [ ] Use strong `SECRET_KEY_BASE` (generated with `lucky gen.secret_key`)
- [ ] Use HTTPS in production (enable `ForceSSLHandler`)
- [ ] Secure PostgreSQL with strong passwords
- [ ] Keep Crystal and dependencies updated
- [ ] Use environment variables for sensitive data (never commit to git)
- [ ] Set up firewall rules to restrict database access
- [ ] Enable CORS only for trusted domains
- [ ] Implement rate limiting (consider adding middleware)
- [ ] Regular security audits
- [ ] Monitor logs for suspicious activity

## Troubleshooting

### Database Connection Errors

```bash
# Check PostgreSQL is running
$ systemctl status postgresql

# Test connection
$ psql -h localhost -U postgres -d paphos_development
```

### Migration Errors

```bash
# Check migration status
$ lucky db.migrate.status

# Rollback and retry
$ lucky db.rollback
$ lucky db.migrate
```

### Port Already in Use

```bash
# Find process using port 3000
$ lsof -i :3000

# Kill process
$ kill -9 <PID>
```

## Scaling Considerations

### Horizontal Scaling

The application is stateless and can be scaled horizontally:

1. Deploy multiple instances behind a load balancer
2. Use a shared PostgreSQL database
3. Ensure `SECRET_KEY_BASE` is the same across all instances

### Database Scaling

- Use PostgreSQL read replicas for read-heavy workloads
- Implement connection pooling (PgBouncer)
- Consider partitioning large tables (messages)
- Add database indexes for common queries

### Caching

Consider adding Redis for:
- Session storage
- Frequently accessed characters
- API response caching

## Support

For issues related to:
- Lucky Framework: https://luckyframework.org/
- Crystal Language: https://crystal-lang.org/
- This project: See repository issues

