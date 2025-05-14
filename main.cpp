#include <iostream>
#include <stdexcept>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>

using namespace std;
using namespace sql;

void crearEstudiante(Connection* conn) {
    string nombre, grado;
    int edad;

    cout << "Nombre: ";
    getline(cin, nombre);
    cout << "Edad: ";
    cin >> edad;
    cin.ignore();
    cout << "Grado: ";
    getline(cin, grado);

    PreparedStatement* pstmt = conn->prepareStatement("INSERT INTO estudiantes(nombre, edad, grado) VALUES (?, ?, ?)");
    pstmt->setString(1, nombre);
    pstmt->setInt(2, edad);
    pstmt->setString(3, grado);
    pstmt->execute();

    cout << "Estudiante registrado.\n";
    delete pstmt;
}

void leerEstudiantes(Connection* conn) {
    Statement* stmt = conn->createStatement();
    ResultSet* res = stmt->executeQuery("SELECT * FROM estudiantes");

    while (res->next()) {
        cout << "ID: " << res->getInt("id")
             << ", Nombre: " << res->getString("nombre")
             << ", Edad: " << res->getInt("edad")
             << ", Grado: " << res->getString("grado") << "\n";
    }

    delete res;
    delete stmt;
}

void actualizarEstudiante(Connection* conn) {
    int id, edad;
    string nombre, grado;

    cout << "ID del estudiante a actualizar: ";
    cin >> id;
    cin.ignore();
    cout << "Nuevo nombre: ";
    getline(cin, nombre);
    cout << "Nueva edad: ";
    cin >> edad;
    cin.ignore();
    cout << "Nuevo grado: ";
    getline(cin, grado);

    PreparedStatement* pstmt = conn->prepareStatement("UPDATE estudiantes SET nombre=?, edad=?, grado=? WHERE id=?");
    pstmt->setString(1, nombre);
    pstmt->setInt(2, edad);
    pstmt->setString(3, grado);
    pstmt->setInt(4, id);
    pstmt->execute();

    cout << "Estudiante actualizado.\n";
    delete pstmt;
}

void eliminarEstudiante(Connection* conn) {
    int id;
    cout << "ID del estudiante a eliminar: ";
    cin >> id;
    cin.ignore();

    PreparedStatement* pstmt = conn->prepareStatement("DELETE FROM estudiantes WHERE id=?");
    pstmt->setInt(1, id);
    pstmt->execute();

    cout << "Estudiante eliminado.\n";
    delete pstmt;
}

int main() {
    try {
        Driver* driver = get_driver_instance();
        Connection* conn = driver->connect("tcp://127.0.0.1:3306", "root", "tu_contraseña");

        conn->setSchema("colegio");

        int opcion;
        do {
            cout << "\n--- Menu ---\n";
            cout << "1. Crear estudiante\n";
            cout << "2. Ver estudiantes\n";
            cout << "3. Actualizar estudiante\n";
            cout << "4. Eliminar estudiante\n";
            cout << "5. Salir\n";
            cout << "Seleccione una opción: ";
            cin >> opcion;
            cin.ignore();

            switch (opcion) {
                case 1: crearEstudiante(conn); break;
                case 2: leerEstudiantes(conn); break;
                case 3: actualizarEstudiante(conn); break;
                case 4: eliminarEstudiante(conn); break;
                case 5: cout << "Saliendo...\n"; break;
                default: cout << "Opción inválida\n"; break;
            }
        } while (opcion != 5);

        delete conn;
    } catch (SQLException& e) {
        cerr << "Error MySQL: " << e.what() << endl;
        return 1;
    } catch (exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
