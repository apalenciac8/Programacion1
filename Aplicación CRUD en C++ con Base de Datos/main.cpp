#define SQL_NOUNICODEMAP
#define UNICODE 0
#ifndef _MBCS
#define _MBCS
#endif

#include <iostream>
#include <string>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#pragma comment(lib, "odbc32.lib")

using namespace std;

// ── Variables globales de conexion ──────────────────────────
SQLHENV  hEnv  = SQL_NULL_HENV;
SQLHDBC  hDbc  = SQL_NULL_HDBC;
SQLHSTMT hStmt = SQL_NULL_HSTMT;

// ── Prototipos ───────────────────────────────────────────────
bool conectar();
void desconectar();
bool ejecutarSQL(const string& sql);
void mostrarError(const string& contexto);

void mostrarMenuPrincipal();
void menuAlumnos();
void menuCursos();
void menuSecciones();
void menuAlumnosCursos();
void menuAlumnosSeccion();

// Alumnos
void listarAlumnos();
void insertarAlumno();
void actualizarAlumno();
void eliminarAlumno();

// Cursos
void listarCursos();
void insertarCurso();
void actualizarCurso();
void eliminarCurso();

// Secciones
void listarSecciones();
void insertarSeccion();
void actualizarSeccion();
void eliminarSeccion();

// Alumnos-Cursos
void listarAlumnosCursos();
void asignarCursoAlumno();
void eliminarAsignacionCurso();

// Alumnos-Seccion
void listarAlumnosSeccion();
void asignarSeccionAlumno();
void modificarSeccionAlumno();
void eliminarAsignacionSeccion();

// ── Helpers ──────────────────────────────────────────────────
void limpiarPantalla() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    cout << "\nPresione Enter para continuar...";
    cin.ignore();
    cin.get();
}

string leerLinea(const string& prompt) {
    string valor;
    cout << prompt;
    getline(cin, valor);
    return valor;
}

int leerEntero(const string& prompt) {
    int valor;
    cout << prompt;
    cin >> valor;
    cin.ignore();
    return valor;
}

// ============================================================
//  CONEXION
// ============================================================
bool conectar() {
    string dsn      = "DSN=universidad_pg;";
    string usuario  = "postgres";
    string password = "Ravenclaw08!";

    SQLRETURN ret;

    ret = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
    if (!SQL_SUCCEEDED(ret)) { cerr << "Error al crear entorno ODBC\n"; return false; }

    SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

    ret = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
    if (!SQL_SUCCEEDED(ret)) { cerr << "Error al crear handle de conexion\n"; return false; }

    ret = SQLDriverConnect(hDbc, NULL,
        (SQLCHAR*)dsn.c_str(), SQL_NTS,
        NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    if (!SQL_SUCCEEDED(ret)) {
        mostrarError("Conexion a la base de datos");
        return false;
    }

    cout << "Conexion exitosa a PostgreSQL (universidad)\n";
    return true;
}

void desconectar() {
    if (hStmt != SQL_NULL_HSTMT) { SQLFreeHandle(SQL_HANDLE_STMT, hStmt); hStmt = SQL_NULL_HSTMT; }
    if (hDbc  != SQL_NULL_HDBC)  { SQLDisconnect(hDbc); SQLFreeHandle(SQL_HANDLE_DBC,  hDbc);  hDbc  = SQL_NULL_HDBC;  }
    if (hEnv  != SQL_NULL_HENV)  { SQLFreeHandle(SQL_HANDLE_ENV,  hEnv);  hEnv  = SQL_NULL_HENV;  }
    cout << "Conexion cerrada correctamente.\n";
}

void mostrarError(const string& contexto) {
    SQLCHAR sqlState[6], msg[SQL_MAX_MESSAGE_LENGTH];
    SQLINTEGER nativeError;
    SQLSMALLINT msgLen;
    cerr << "Error en [" << contexto << "]: ";
    SQLGetDiagRec(SQL_HANDLE_DBC, hDbc, 1, sqlState, &nativeError, msg, sizeof(msg), &msgLen);
    cerr << msg << "\n";
}

bool ejecutarSQL(const string& sql) {
    if (hStmt != SQL_NULL_HSTMT) { SQLFreeHandle(SQL_HANDLE_STMT, hStmt); hStmt = SQL_NULL_HSTMT; }
    SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
    SQLRETURN ret = SQLExecDirect(hStmt, (SQLCHAR*)sql.c_str(), SQL_NTS);
    if (!SQL_SUCCEEDED(ret)) {
        SQLCHAR sqlState[6], msg[SQL_MAX_MESSAGE_LENGTH];
        SQLINTEGER nativeError; SQLSMALLINT msgLen;
        SQLGetDiagRec(SQL_HANDLE_STMT, hStmt, 1, sqlState, &nativeError, msg, sizeof(msg), &msgLen);
        cerr << "SQL Error: " << msg << "\n";
        return false;
    }
    return true;
}

// ============================================================
//  MENU PRINCIPAL
// ============================================================
void mostrarMenuPrincipal() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "===== SISTEMA UNIVERSIDAD =====\n";
        cout << "1. Alumnos\n";
        cout << "2. Cursos\n";
        cout << "3. Secciones\n";
        cout << "4. Alumnos - Cursos\n";
        cout << "5. Alumnos - Seccion\n";
        cout << "6. Salir\n";
        cout << "================================\n";
        opcion = leerEntero("Seleccione una opcion: ");

        switch (opcion) {
            case 1: menuAlumnos();       break;
            case 2: menuCursos();        break;
            case 3: menuSecciones();     break;
            case 4: menuAlumnosCursos(); break;
            case 5: menuAlumnosSeccion();break;
            case 6: cout << "Cerrando sistema...\n"; break;
            default: cout << "Opcion invalida.\n"; pausar();
        }
    } while (opcion != 6);
}

// ============================================================
//  MODULO ALUMNOS
// ============================================================
void menuAlumnos() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "===== MODULO ALUMNOS =====\n";
        cout << "1. Listar\n2. Grabar\n3. Modificar\n4. Eliminar\n5. Regresar\n";
        cout << "===========================\n";
        opcion = leerEntero("Seleccione: ");
        switch (opcion) {
            case 1: listarAlumnos();   break;
            case 2: insertarAlumno();  break;
            case 3: actualizarAlumno();break;
            case 4: eliminarAlumno();  break;
            case 5: break;
            default: cout << "Opcion invalida.\n"; pausar();
        }
    } while (opcion != 5);
}

void listarAlumnos() {
    limpiarPantalla();
    cout << "===== LISTADO DE ALUMNOS =====\n\n";

    string sql =
        "SELECT a.id, a.nombres, a.apellidos, a.carnet, "
        "       COALESCE(c.nombre, 'Sin cursos') AS curso, "
        "       COALESCE(s.nombre, 'Sin seccion') AS seccion "
        "FROM alumnos a "
        "LEFT JOIN alumnos_cursos ac ON a.id = ac.alumno_id "
        "LEFT JOIN cursos c ON ac.curso_id = c.id "
        "LEFT JOIN alumnos_seccion ase ON a.id = ase.alumno_id "
        "LEFT JOIN secciones s ON ase.seccion_id = s.id "
        "ORDER BY a.id;";

    if (!ejecutarSQL(sql)) { pausar(); return; }

    char id[10], nombres[100], apellidos[100], carnet[30], curso[100], seccion[100];
    SQLLEN ind;

    printf("%-5s %-20s %-20s %-15s %-20s %-15s\n",
           "ID", "Nombres", "Apellidos", "Carnet", "Curso", "Seccion");
    printf("%s\n", string(95, '-').c_str());

    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt,1,SQL_C_CHAR,id,      sizeof(id),      &ind);
        SQLGetData(hStmt,2,SQL_C_CHAR,nombres,  sizeof(nombres),  &ind);
        SQLGetData(hStmt,3,SQL_C_CHAR,apellidos,sizeof(apellidos),&ind);
        SQLGetData(hStmt,4,SQL_C_CHAR,carnet,   sizeof(carnet),   &ind);
        SQLGetData(hStmt,5,SQL_C_CHAR,curso,    sizeof(curso),    &ind);
        SQLGetData(hStmt,6,SQL_C_CHAR,seccion,  sizeof(seccion),  &ind);
        printf("%-5s %-20s %-20s %-15s %-20s %-15s\n",
               id, nombres, apellidos, carnet, curso, seccion);
    }
    pausar();
}

void insertarAlumno() {
    limpiarPantalla();
    cout << "===== NUEVO ALUMNO =====\n";
    string nombres   = leerLinea("Nombres   : ");
    string apellidos = leerLinea("Apellidos : ");
    string carnet    = leerLinea("Carnet    : ");

    string sql = "INSERT INTO alumnos (nombres, apellidos, carnet) VALUES ('" +
                 nombres + "','" + apellidos + "','" + carnet + "');";

    if (ejecutarSQL(sql)) cout << "Alumno grabado correctamente.\n";
    pausar();
}

void actualizarAlumno() {
    limpiarPantalla();
    cout << "===== MODIFICAR ALUMNO =====\n";
    listarAlumnos();
    int id = leerEntero("ID del alumno a modificar: ");
    string nombres   = leerLinea("Nuevos nombres   : ");
    string apellidos = leerLinea("Nuevos apellidos : ");
    string carnet    = leerLinea("Nuevo carnet     : ");

    string sql = "UPDATE alumnos SET nombres='" + nombres +
                 "', apellidos='" + apellidos +
                 "', carnet='" + carnet +
                 "' WHERE id=" + to_string(id) + ";";

    if (ejecutarSQL(sql)) cout << "Alumno actualizado.\n";
    pausar();
}

void eliminarAlumno() {
    limpiarPantalla();
    cout << "===== ELIMINAR ALUMNO =====\n";
    listarAlumnos();
    int id = leerEntero("ID del alumno a eliminar: ");

    // Eliminar relaciones primero
    ejecutarSQL("DELETE FROM alumnos_cursos  WHERE alumno_id=" + to_string(id) + ";");
    ejecutarSQL("DELETE FROM alumnos_seccion WHERE alumno_id=" + to_string(id) + ";");

    if (ejecutarSQL("DELETE FROM alumnos WHERE id=" + to_string(id) + ";"))
        cout << "Alumno eliminado.\n";
    pausar();
}

// ============================================================
//  MODULO CURSOS
// ============================================================
void menuCursos() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "===== MODULO CURSOS =====\n";
        cout << "1. Listar\n2. Grabar\n3. Modificar\n4. Eliminar\n5. Regresar\n";
        cout << "==========================\n";
        opcion = leerEntero("Seleccione: ");
        switch (opcion) {
            case 1: listarCursos();   break;
            case 2: insertarCurso();  break;
            case 3: actualizarCurso();break;
            case 4: eliminarCurso();  break;
            case 5: break;
            default: cout << "Opcion invalida.\n"; pausar();
        }
    } while (opcion != 5);
}

void listarCursos() {
    limpiarPantalla();
    cout << "===== LISTADO DE CURSOS =====\n\n";
    if (!ejecutarSQL("SELECT id, nombre, codigo FROM cursos ORDER BY id;")) { pausar(); return; }

    char id[10], nombre[100], codigo[30];
    SQLLEN ind;
    printf("%-5s %-30s %-15s\n", "ID", "Nombre", "Codigo");
    printf("%s\n", string(52, '-').c_str());
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt,1,SQL_C_CHAR,id,    sizeof(id),    &ind);
        SQLGetData(hStmt,2,SQL_C_CHAR,nombre,sizeof(nombre),&ind);
        SQLGetData(hStmt,3,SQL_C_CHAR,codigo,sizeof(codigo),&ind);
        printf("%-5s %-30s %-15s\n", id, nombre, codigo);
    }
    pausar();
}

void insertarCurso() {
    limpiarPantalla();
    cout << "===== NUEVO CURSO =====\n";
    string nombre = leerLinea("Nombre : ");
    string codigo = leerLinea("Codigo : ");
    string sql = "INSERT INTO cursos (nombre, codigo) VALUES ('" + nombre + "','" + codigo + "');";
    if (ejecutarSQL(sql)) cout << "Curso grabado.\n";
    pausar();
}

void actualizarCurso() {
    limpiarPantalla();
    cout << "===== MODIFICAR CURSO =====\n";
    listarCursos();
    int id = leerEntero("ID del curso a modificar: ");
    string nombre = leerLinea("Nuevo nombre : ");
    string codigo = leerLinea("Nuevo codigo : ");
    string sql = "UPDATE cursos SET nombre='" + nombre + "', codigo='" + codigo +
                 "' WHERE id=" + to_string(id) + ";";
    if (ejecutarSQL(sql)) cout << "Curso actualizado.\n";
    pausar();
}

void eliminarCurso() {
    limpiarPantalla();
    cout << "===== ELIMINAR CURSO =====\n";
    listarCursos();
    int id = leerEntero("ID del curso a eliminar: ");

    // Validar si esta asignado a alumnos
    if (!ejecutarSQL("SELECT COUNT(*) FROM alumnos_cursos WHERE curso_id=" + to_string(id) + ";")) { pausar(); return; }
    char cnt[10]; SQLLEN ind;
    SQLFetch(hStmt);
    SQLGetData(hStmt,1,SQL_C_CHAR,cnt,sizeof(cnt),&ind);
    if (atoi(cnt) > 0) {
        cout << "No se puede eliminar: el curso esta asignado a " << cnt << " alumno(s).\n";
        pausar(); return;
    }

    if (ejecutarSQL("DELETE FROM cursos WHERE id=" + to_string(id) + ";"))
        cout << "Curso eliminado.\n";
    pausar();
}

// ============================================================
//  MODULO SECCIONES
// ============================================================
void menuSecciones() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "===== MODULO SECCIONES =====\n";
        cout << "1. Listar\n2. Grabar\n3. Modificar\n4. Eliminar\n5. Regresar\n";
        cout << "=============================\n";
        opcion = leerEntero("Seleccione: ");
        switch (opcion) {
            case 1: listarSecciones();   break;
            case 2: insertarSeccion();   break;
            case 3: actualizarSeccion(); break;
            case 4: eliminarSeccion();   break;
            case 5: break;
            default: cout << "Opcion invalida.\n"; pausar();
        }
    } while (opcion != 5);
}

void listarSecciones() {
    limpiarPantalla();
    cout << "===== LISTADO DE SECCIONES =====\n\n";
    if (!ejecutarSQL("SELECT id, nombre, jornada FROM secciones ORDER BY id;")) { pausar(); return; }

    char id[10], nombre[100], jornada[50];
    SQLLEN ind;
    printf("%-5s %-20s %-15s\n", "ID", "Nombre", "Jornada");
    printf("%s\n", string(42, '-').c_str());
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt,1,SQL_C_CHAR,id,     sizeof(id),     &ind);
        SQLGetData(hStmt,2,SQL_C_CHAR,nombre, sizeof(nombre), &ind);
        SQLGetData(hStmt,3,SQL_C_CHAR,jornada,sizeof(jornada),&ind);
        printf("%-5s %-20s %-15s\n", id, nombre, jornada);
    }
    pausar();
}

void insertarSeccion() {
    limpiarPantalla();
    cout << "===== NUEVA SECCION =====\n";
    string nombre  = leerLinea("Nombre  : ");
    string jornada = leerLinea("Jornada : ");
    string sql = "INSERT INTO secciones (nombre, jornada) VALUES ('" + nombre + "','" + jornada + "');";
    if (ejecutarSQL(sql)) cout << "Seccion grabada.\n";
    pausar();
}

void actualizarSeccion() {
    limpiarPantalla();
    cout << "===== MODIFICAR SECCION =====\n";
    listarSecciones();
    int id = leerEntero("ID de la seccion a modificar: ");
    string nombre  = leerLinea("Nuevo nombre  : ");
    string jornada = leerLinea("Nueva jornada : ");
    string sql = "UPDATE secciones SET nombre='" + nombre + "', jornada='" + jornada +
                 "' WHERE id=" + to_string(id) + ";";
    if (ejecutarSQL(sql)) cout << "Seccion actualizada.\n";
    pausar();
}

void eliminarSeccion() {
    limpiarPantalla();
    cout << "===== ELIMINAR SECCION =====\n";
    listarSecciones();
    int id = leerEntero("ID de la seccion a eliminar: ");

    // Validar si tiene alumnos
    if (!ejecutarSQL("SELECT COUNT(*) FROM alumnos_seccion WHERE seccion_id=" + to_string(id) + ";")) { pausar(); return; }
    char cnt[10]; SQLLEN ind;
    SQLFetch(hStmt);
    SQLGetData(hStmt,1,SQL_C_CHAR,cnt,sizeof(cnt),&ind);
    if (atoi(cnt) > 0) {
        cout << "No se puede eliminar: la seccion tiene " << cnt << " alumno(s) asignado(s).\n";
        pausar(); return;
    }

    if (ejecutarSQL("DELETE FROM secciones WHERE id=" + to_string(id) + ";"))
        cout << "Seccion eliminada.\n";
    pausar();
}

// ============================================================
//  MODULO ALUMNOS - CURSOS
// ============================================================
void menuAlumnosCursos() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "===== MODULO ALUMNOS - CURSOS =====\n";
        cout << "1. Listar asignaciones\n2. Asignar curso a alumno\n3. Eliminar asignacion\n4. Regresar\n";
        cout << "====================================\n";
        opcion = leerEntero("Seleccione: ");
        switch (opcion) {
            case 1: listarAlumnosCursos();     break;
            case 2: asignarCursoAlumno();      break;
            case 3: eliminarAsignacionCurso(); break;
            case 4: break;
            default: cout << "Opcion invalida.\n"; pausar();
        }
    } while (opcion != 4);
}

void listarAlumnosCursos() {
    limpiarPantalla();
    cout << "===== ASIGNACIONES ALUMNOS - CURSOS =====\n\n";
    string sql =
        "SELECT ac.id, a.nombres || ' ' || a.apellidos AS alumno, c.nombre AS curso "
        "FROM alumnos_cursos ac "
        "JOIN alumnos a ON ac.alumno_id = a.id "
        "JOIN cursos  c ON ac.curso_id  = c.id "
        "ORDER BY ac.id;";
    if (!ejecutarSQL(sql)) { pausar(); return; }

    char id[10], alumno[150], curso[100];
    SQLLEN ind;
    printf("%-5s %-35s %-30s\n", "ID", "Alumno", "Curso");
    printf("%s\n", string(72, '-').c_str());
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt,1,SQL_C_CHAR,id,    sizeof(id),    &ind);
        SQLGetData(hStmt,2,SQL_C_CHAR,alumno,sizeof(alumno),&ind);
        SQLGetData(hStmt,3,SQL_C_CHAR,curso, sizeof(curso), &ind);
        printf("%-5s %-35s %-30s\n", id, alumno, curso);
    }
    pausar();
}

void asignarCursoAlumno() {
    limpiarPantalla();
    cout << "===== ASIGNAR CURSO A ALUMNO =====\n";
    listarAlumnos();
    int alumnoId = leerEntero("ID del alumno: ");
    listarCursos();
    int cursoId  = leerEntero("ID del curso : ");

    // Evitar duplicado
    if (!ejecutarSQL("SELECT COUNT(*) FROM alumnos_cursos WHERE alumno_id=" +
                     to_string(alumnoId) + " AND curso_id=" + to_string(cursoId) + ";")) {
        pausar(); return;
    }
    char cnt[10]; SQLLEN ind;
    SQLFetch(hStmt);
    SQLGetData(hStmt,1,SQL_C_CHAR,cnt,sizeof(cnt),&ind);
    if (atoi(cnt) > 0) {
        cout << "El alumno ya tiene asignado ese curso.\n";
        pausar(); return;
    }

    string sql = "INSERT INTO alumnos_cursos (alumno_id, curso_id) VALUES (" +
                 to_string(alumnoId) + "," + to_string(cursoId) + ");";
    if (ejecutarSQL(sql)) cout << "Curso asignado correctamente.\n";
    pausar();
}

void eliminarAsignacionCurso() {
    limpiarPantalla();
    cout << "===== ELIMINAR ASIGNACION CURSO =====\n";
    listarAlumnosCursos();
    int id = leerEntero("ID de la asignacion a eliminar: ");
    if (ejecutarSQL("DELETE FROM alumnos_cursos WHERE id=" + to_string(id) + ";"))
        cout << "Asignacion eliminada.\n";
    pausar();
}

// ============================================================
//  MODULO ALUMNOS - SECCION
// ============================================================
void menuAlumnosSeccion() {
    int opcion;
    do {
        limpiarPantalla();
        cout << "===== MODULO ALUMNOS - SECCION =====\n";
        cout << "1. Listar asignaciones\n2. Asignar seccion a alumno\n"
             << "3. Modificar seccion\n4. Eliminar asignacion\n5. Regresar\n";
        cout << "=====================================\n";
        opcion = leerEntero("Seleccione: ");
        switch (opcion) {
            case 1: listarAlumnosSeccion();     break;
            case 2: asignarSeccionAlumno();     break;
            case 3: modificarSeccionAlumno();   break;
            case 4: eliminarAsignacionSeccion();break;
            case 5: break;
            default: cout << "Opcion invalida.\n"; pausar();
        }
    } while (opcion != 5);
}

void listarAlumnosSeccion() {
    limpiarPantalla();
    cout << "===== ASIGNACIONES ALUMNOS - SECCION =====\n\n";
    string sql =
        "SELECT ase.id, a.nombres || ' ' || a.apellidos AS alumno, s.nombre AS seccion, s.jornada "
        "FROM alumnos_seccion ase "
        "JOIN alumnos  a ON ase.alumno_id  = a.id "
        "JOIN secciones s ON ase.seccion_id = s.id "
        "ORDER BY ase.id;";
    if (!ejecutarSQL(sql)) { pausar(); return; }

    char id[10], alumno[150], seccion[100], jornada[50];
    SQLLEN ind;
    printf("%-5s %-35s %-20s %-15s\n", "ID", "Alumno", "Seccion", "Jornada");
    printf("%s\n", string(77, '-').c_str());
    while (SQLFetch(hStmt) == SQL_SUCCESS) {
        SQLGetData(hStmt,1,SQL_C_CHAR,id,     sizeof(id),     &ind);
        SQLGetData(hStmt,2,SQL_C_CHAR,alumno, sizeof(alumno), &ind);
        SQLGetData(hStmt,3,SQL_C_CHAR,seccion,sizeof(seccion),&ind);
        SQLGetData(hStmt,4,SQL_C_CHAR,jornada,sizeof(jornada),&ind);
        printf("%-5s %-35s %-20s %-15s\n", id, alumno, seccion, jornada);
    }
    pausar();
}

void asignarSeccionAlumno() {
    limpiarPantalla();
    cout << "===== ASIGNAR SECCION A ALUMNO =====\n";
    listarAlumnos();
    int alumnoId = leerEntero("ID del alumno  : ");

    // Verificar que no tenga ya una seccion activa
    if (!ejecutarSQL("SELECT COUNT(*) FROM alumnos_seccion WHERE alumno_id=" + to_string(alumnoId) + ";")) {
        pausar(); return;
    }
    char cnt[10]; SQLLEN ind;
    SQLFetch(hStmt);
    SQLGetData(hStmt,1,SQL_C_CHAR,cnt,sizeof(cnt),&ind);
    if (atoi(cnt) > 0) {
        cout << "El alumno ya tiene una seccion activa. Use 'Modificar seccion'.\n";
        pausar(); return;
    }

    listarSecciones();
    int seccionId = leerEntero("ID de la seccion: ");
    string sql = "INSERT INTO alumnos_seccion (alumno_id, seccion_id) VALUES (" +
                 to_string(alumnoId) + "," + to_string(seccionId) + ");";
    if (ejecutarSQL(sql)) cout << "Seccion asignada correctamente.\n";
    pausar();
}

void modificarSeccionAlumno() {
    limpiarPantalla();
    cout << "===== MODIFICAR SECCION DE ALUMNO =====\n";
    listarAlumnosSeccion();
    int id = leerEntero("ID de la asignacion a modificar: ");
    listarSecciones();
    int seccionId = leerEntero("Nueva seccion ID: ");
    string sql = "UPDATE alumnos_seccion SET seccion_id=" + to_string(seccionId) +
                 " WHERE id=" + to_string(id) + ";";
    if (ejecutarSQL(sql)) cout << "Seccion actualizada.\n";
    pausar();
}

void eliminarAsignacionSeccion() {
    limpiarPantalla();
    cout << "===== ELIMINAR ASIGNACION SECCION =====\n";
    listarAlumnosSeccion();
    int id = leerEntero("ID de la asignacion a eliminar: ");
    if (ejecutarSQL("DELETE FROM alumnos_seccion WHERE id=" + to_string(id) + ";"))
        cout << "Asignacion eliminada.\n";
    pausar();
}

int main() {
    cout << "Iniciando Sistema Universidad...\n";
    if (!conectar()) {
        cerr << "No se pudo conectar a la base de datos. Verifique la configuracion ODBC.\n";
        return 1;
    }
    mostrarMenuPrincipal();
    desconectar();
    return 0;
}
