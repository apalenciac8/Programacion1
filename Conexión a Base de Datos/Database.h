#pragma once
#include <string>
#include <pqxx/pqxx>

// Gestiona la conexión a PostgreSQL usando libpqxx.
// Provee acceso al objeto de conexión para que los repositorios ejecuten queries.
class Database {
public:
    // connection_string: "host=localhost dbname=demo user=postgres password=secret"
    explicit Database(const std::string& connection_string);
    ~Database() = default;

    // Devuelve la conexión activa (no thread-safe por diseño simplificado)
    pqxx::connection& connection();

    // Verifica que la conexión esté activa
    bool isConnected() const;

private:
    pqxx::connection conn_;
};
