# C++ + PostgreSQL Demo

Proyecto de ejemplo que muestra cómo conectar C++17 a PostgreSQL usando **libpqxx**.

## Estructura

```
cpp_postgres_demo/
├── CMakeLists.txt
├── README.md
├── sql/
│   └── setup.sql          ← Script para crear la base de datos
├── include/
│   ├── User.h             ← Modelo de datos
│   ├── Database.h         ← Wrapper de conexión
│   └── UserRepository.h   ← Interfaz CRUD
└── src/
    ├── main.cpp           ← Punto de entrada + demo
    ├── Database.cpp
    └── UserRepository.cpp
```

## Dependencias

- CMake ≥ 3.16
- Compilador con soporte C++17 (GCC 9+ / Clang 10+)
- PostgreSQL (servidor + headers de desarrollo)
- libpqxx ≥ 7.x

### Ubuntu / Debian

```bash
sudo apt update
sudo apt install -y cmake g++ libpq-dev libpqxx-dev postgresql
```

### macOS (Homebrew)

```bash
brew install cmake postgresql libpqxx
```

### Windows (vcpkg)

```powershell
vcpkg install libpqxx:x64-windows
```

## Configurar la base de datos

```bash
# Iniciar PostgreSQL (Ubuntu)
sudo service postgresql start

# Crear la base de datos
sudo -u postgres psql -f sql/setup.sql
```

## Compilar y ejecutar

```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Ajusta la cadena de conexión en src/main.cpp si es necesario
./build/demo
```

## Salida esperada

```
[DB] Conectado a: demo_db
[Repo] Tabla 'usuarios' lista.

=== Insertando usuarios ===
[Repo] Usuario creado con id=1
...

=== Lista de usuarios ===
  ID | Nombre              | Email                        | Creado
  ---+---------------------+------------------------------+---------------------------
  1  | Ana García          | ana@example.com              | 2025-04-17 10:00:00+00
  2  | Carlos López        | carlos@example.com           | 2025-04-17 10:00:00+00
  3  | María Torres        | maria@example.com            | 2025-04-17 10:00:00+00
...
```

## Cambiar los parámetros de conexión

Edita la variable `conn_str` en `src/main.cpp`, o usa variables de entorno:

```cpp
std::string conn_str = std::string("host=") + std::getenv("DB_HOST") +
                       " dbname=" + std::getenv("DB_NAME") + ...;
```

## Seguridad

Todas las consultas usan parámetros posicionales (`$1`, `$2`) — **nunca concatenación de strings** — lo que previene inyección SQL.
