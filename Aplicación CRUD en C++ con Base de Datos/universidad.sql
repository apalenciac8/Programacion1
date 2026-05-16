-- ============================================================
--  Crear tablas
-- ===========================================-- Tabla: alumnos
CREATE TABLE IF NOT EXISTS alumnos (
    id        SERIAL PRIMARY KEY,
    nombres   VARCHAR(100) NOT NULL,
    apellidos VARCHAR(100) NOT NULL,
    carnet    VARCHAR(30)  NOT NULL UNIQUE
);

-- Tabla: cursos
CREATE TABLE IF NOT EXISTS cursos (
    id     SERIAL PRIMARY KEY,
    nombre VARCHAR(100) NOT NULL,
    codigo VARCHAR(30)  NOT NULL UNIQUE
);

-- Tabla: secciones
CREATE TABLE IF NOT EXISTS secciones (
    id      SERIAL PRIMARY KEY,
    nombre  VARCHAR(50) NOT NULL,
    jornada VARCHAR(50) NOT NULL
);

-- Tabla relacion: alumnos_cursos
CREATE TABLE IF NOT EXISTS alumnos_cursos (
    id         SERIAL PRIMARY KEY,
    alumno_id  INT NOT NULL REFERENCES alumnos(id),
    curso_id   INT NOT NULL REFERENCES cursos(id),
    UNIQUE (alumno_id, curso_id)   -- un alumno no puede tener el mismo curso dos veces
);

-- Tabla relacion: alumnos_seccion
CREATE TABLE IF NOT EXISTS alumnos_seccion (
    id          SERIAL PRIMARY KEY,
    alumno_id   INT NOT NULL REFERENCES alumnos(id) UNIQUE, -- max 1 seccion por alumno
    seccion_id  INT NOT NULL REFERENCES secciones(id)
);

-- ============================================================
--  3. Datos de ejemplo (opcional pero recomendado para probar)
-- ============================================================

-- Alumnos
INSERT INTO alumnos (nombres, apellidos, carnet) VALUES
    ('Carlos Alberto',  'Lopez Ramirez',  '2024-001'),
    ('Maria Fernanda',  'Garcia Morales', '2024-002'),
    ('Jose David',      'Martinez Cruz',  '2024-003'),
    ('Ana Lucia',       'Perez Gomez',    '2024-004');

-- Cursos
INSERT INTO cursos (nombre, codigo) VALUES
    ('Programacion I',     'PROG-101'),
    ('Matematica Basica',  'MAT-101'),
    ('Ingles Tecnico',     'ING-101'),
    ('Base de Datos',      'DB-201');

-- Secciones
INSERT INTO secciones (nombre, jornada) VALUES
    ('Seccion A', 'Matutina'),
    ('Seccion B', 'Vespertina'),
    ('Seccion C', 'Nocturna');

-- Asignacion alumnos -> cursos
INSERT INTO alumnos_cursos (alumno_id, curso_id) VALUES
    (1, 1), (1, 2),   -- Carlos: Prog I y Matematica
    (2, 1), (2, 3),   -- Maria: Prog I e Ingles
    (3, 4),            -- Jose: Base de Datos
    (4, 1);            -- Ana: Prog I

-- Asignacion alumnos -> seccion
INSERT INTO alumnos_seccion (alumno_id, seccion_id) VALUES
    (1, 1),   -- Carlos en Seccion A
    (2, 1),   -- Maria en Seccion A
    (3, 2),   -- Jose en Seccion B
    (4, 3);   -- Ana en Seccion C

-- ============================================================
--  4. Consulta de verificacion (listado completo de alumnos)
-- ============================================================
SELECT
    a.id,
    a.nombres,
    a.apellidos,
    a.carnet,
    COALESCE(c.nombre, 'Sin cursos')   AS curso,
    COALESCE(s.nombre, 'Sin seccion')  AS seccion
FROM alumnos a
LEFT JOIN alumnos_cursos  ac  ON a.id = ac.alumno_id
LEFT JOIN cursos           c  ON ac.curso_id   = c.id
LEFT JOIN alumnos_seccion ase ON a.id = ase.alumno_id
LEFT JOIN secciones        s  ON ase.seccion_id = s.id
ORDER BY a.id, c.nombre;
