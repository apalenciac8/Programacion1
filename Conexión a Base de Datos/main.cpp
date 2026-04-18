#include <iostream>
#include <stdexcept>
#include "Database.h"
#include "UserRepository.h"

// Imprime todos los usuarios en formato tabla
void printUsers(const std::vector<User>& users) {
    if (users.empty()) {
        std::cout << "  (sin usuarios)\n";
        return;
    }
    std::cout << "  ID | Nombre              | Email                        | Creado\n";
    std::cout << "  ---+---------------------+------------------------------+---------------------------\n";
    for (const auto& u : users) {
        std::printf("  %-3d| %-20s| %-29s| %s\n",
            u.id, u.nombre.c_str(), u.email.c_str(), u.created_at.c_str());
    }
}

int main() {
    // ── Configuración de conexión ────────────────────────────────────────────
    // Ajusta estos valores a tu entorno. También puedes usar variables de
    // entorno con: std::getenv("DB_HOST"), etc.
    const std::string conn_str =
        "host=localhost "
        "port=5432 "
        "dbname=demo_db "
        "user=postgres "
        "password=Ravenclaw08!";

    try {
        // ── Conectar ─────────────────────────────────────────────────────────
        Database db(conn_str);
        UserRepository repo(db);

        // ── Crear tabla ───────────────────────────────────────────────────────
        repo.createTableIfNotExists();

        // ── CREATE ────────────────────────────────────────────────────────────
        std::cout << "\n=== Insertando usuarios ===\n";
        int id1 = repo.create("Ana García",    "ana@example.com");
        int id2 = repo.create("Carlos López",  "carlos@example.com");
        int id3 = repo.create("María Torres",  "maria@example.com");

        // ── READ (todos) ──────────────────────────────────────────────────────
        std::cout << "\n=== Lista de usuarios ===\n";
        printUsers(repo.findAll());

        // ── READ (por ID) ─────────────────────────────────────────────────────
        std::cout << "\n=== Buscar usuario id=" << id1 << " ===\n";
        if (auto u = repo.findById(id1)) {
            std::cout << "  Encontrado: " << u->nombre << " <" << u->email << ">\n";
        }

        // ── UPDATE ────────────────────────────────────────────────────────────
        std::cout << "\n=== Actualizar usuario id=" << id2 << " ===\n";
        bool updated = repo.update(id2, "Carlos Martínez", "c.martinez@example.com");
        std::cout << "  Actualizado: " << (updated ? "sí" : "no") << "\n";

        // ── DELETE ────────────────────────────────────────────────────────────
        std::cout << "\n=== Eliminar usuario id=" << id3 << " ===\n";
        bool removed = repo.remove(id3);
        std::cout << "  Eliminado: " << (removed ? "sí" : "no") << "\n";

        // ── Estado final ──────────────────────────────────────────────────────
        std::cout << "\n=== Estado final ===\n";
        printUsers(repo.findAll());

    } catch (const pqxx::sql_error& e) {
        std::cerr << "[Error SQL] " << e.what()
                  << "\nQuery: " << e.query() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "[Error] " << e.what() << "\n";
        return 1;
    }

    return 0;
}
