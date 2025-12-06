# paphos-backend

This is the core backend for the official Pygmalion service - a character-based chat application API.

## Status

✅ **Fully Implemented** - Core API is complete and functional with comprehensive features for user authentication, character management, chat creation, and messaging.

## Contributing

If you wish to contribute, this section contains some relevant information.

The stack is:

- [Crystal](https://crystal-lang.org/) as the language of choice
- [Lucky](https://luckyframework.org/) as the web framework
- [PostgreSQL](https://www.postgresql.org/) as the main database

To get started, you need all of the above installed and functional on your machine.

For development, the [default configuration](./config/database.cr) expects a `postgres` user with password `postgres` to be available. You can then create the databases manually, or use `lucky db.setup` to create them automatically I believe.

With PostgreSQL up and running and the development database created, you can boot up the development server:

```bash
$ lucky dev
```

The API should then be reachable at http://localhost:3000/. All routes are under `/api/v1` and are fully documented in [API.md](./API.md).

## Features

### Authentication & Users
- ✅ User registration and login with JWT tokens
- ✅ Password encryption using BCrypt
- ✅ Token-based authentication (header or query param)
- ✅ Current user profile endpoint

### Characters
- ✅ Create, read, update, delete characters
- ✅ Character visibility levels (public, unlisted, private)
- ✅ Slug-based character URLs
- ✅ Paginated character listing
- ✅ Character search with contentious filtering
- ✅ Protection: Cannot delete characters used in chats
- ✅ Protection: Cannot make characters private if used in other users' chats

### Chats
- ✅ Create chats with multiple character participants
- ✅ List, show, update, and delete chats
- ✅ Paginated chat listing (sorted by last update)
- ✅ Automatic cascade deletion of messages when chat is deleted

### Messages
- ✅ Send messages in chats
- ✅ List messages with pagination (newest first)
- ✅ Support for both user and bot messages
- ✅ Character attribution for messages
- ✅ Message validation (1-4096 characters)

### Additional Features
- ✅ Comprehensive error handling with detailed messages
- ✅ Input validation on all operations
- ✅ PostgreSQL database with proper relationships
- ✅ Database migrations
- ✅ RESTful API design
- ✅ Full API documentation

## API Documentation

See [API.md](./API.md) for complete API endpoint documentation including:
- Authentication flows
- All available endpoints
- Request/response formats
- Error handling
- Data models

## Quick Start with Docker

```bash
# Start PostgreSQL and the development server
$ docker-compose up
```

The API will be available at http://localhost:5001/api/v1

## Database Migrations

To run migrations:

```bash
$ lucky db.create
$ lucky db.migrate
```

To rollback:

```bash
$ lucky db.rollback
```

### Useful links

- [API Documentation](./API.md) - Complete endpoint reference
- [Lucky's official guides](https://luckyframework.org/guides/getting-started/installing): contains tutorials about the usual stuff (installation, route setup, handling data in the database, etc.)
