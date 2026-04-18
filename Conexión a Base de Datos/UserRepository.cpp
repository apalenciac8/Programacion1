#include "UserRepository.h"
#include <iostream>

UserRepository::UserRepository(Database& db)
    : db_(db) {}

// ── DDL ────────────────────────────────────────────────────────────────────

void UserRepository::createTableIfNotExists() {
    pqxx::work txn(db_.connection());
    txn.exec(R"(
        CREATE TABLE IF NOT EXISTS usuarios (
            id         SERIAL PRIMARY KEY,
            nombre     VARCHAR(100) NOT NULL,
            email      VARCHAR(150) UNIQUE NOT NULL,
            created_at TIMESTAMPTZ  DEFAULT NOW()
        )
    )");
    txn.commit();
    std::cout << "[Repo] Tabla 'usuarios' lista.\n";
}

// ── CREATE ─────────────────────────────────────────────────────────────────

int UserRepository::create(const std::string& nombre, const std::string& email) {
    pqxx::work txn(db_.connection());

    // Usamos parámetros con $1, $2 para evitar SQL injection
    auto result = txn.exec_params(
        "INSERT INTO usuarios (nombre, email) VALUES ($1, $2) RETURNING id",
        nombre, email
    );
    txn.commit();

    int new_id = result[0][0].as<int>();
    std::cout << "[Repo] Usuario creado con id=" << new_id << "\n";
    return new_id;
}

// ── READ ───────────────────────────────────────────────────────────────────

std::vector<User> UserRepository::findAll() {
    pqxx::work txn(db_.connection());
    auto rows = txn.exec("SELECT id, nombre, email, created_at FROM usuarios ORDER BY id");
    txn.commit();

    std::vector<User> usuarios;
    usuarios.reserve(rows.size());
    for (const auto& row : rows) {
        usuarios.push_back({
            row["id"].as<int>(),
            row["nombre"].c_str(),
            row["email"].c_str(),
            row["created_at"].c_str()
        });
    }
    return usuarios;
}

std::optional<User> UserRepository::findById(int id) {
    pqxx::work txn(db_.connection());
    auto rows = txn.exec_params(
        "SELECT id, nombre, email, created_at FROM usuarios WHERE id = $1",
        id
    );
    txn.commit();

    if (rows.empty()) return std::nullopt;

    const auto& row = rows[0];
    return User{
        row["id"].as<int>(),
        row["nombre"].c_str(),
        row["email"].c_str(),
        row["created_at"].c_str()
    };
}

// ── UPDATE ─────────────────────────────────────────────────────────────────

bool UserRepository::update(int id, const std::string& nombre, const std::string& email) {
    pqxx::work txn(db_.connection());
    auto result = txn.exec_params(
        "UPDATE usuarios SET nombre=$1, email=$2 WHERE id=$3",
        nombre, email, id
    );
    txn.commit();
    return result.affected_rows() > 0;
}

// ── DELETE ─────────────────────────────────────────────────────────────────

bool UserRepository::remove(int id) {
    pqxx::work txn(db_.connection());
    auto result = txn.exec_params(
        "DELETE FROM usuarios WHERE id = $1",
        id
    );
    txn.commit();
    return result.affected_rows() > 0;
}
