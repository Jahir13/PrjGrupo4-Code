#include <iostream>
#include <fstream>
#include <iomanip>
#include <unistd.h>
#include <cctype>
#include <windows.h>
#include <vector>
using namespace std;

const string LIMPIAR = "\x1B[2J\x1B[H";
const string ARCHIVORESERVACIONES = "../data/reservacion.txt";
const string ARCHIVOCLIENTESGENERAL = "../data/clienteGeneral.txt";
const string ARCHIVOTAG = "../data/TAG.txt";

struct Cliente
{
    int ID;
    string Cedula;
    string Nombre;
    string Direccion;
    int Asistencia;
};

struct Reservacion
{
    int ID_Cliente;
    string Nombre;
    int Dias;
    string TAG;
    string Check;
};

vector<Cliente> clientes;
vector<Reservacion> reservaciones;

enum color {azul=1, verde, turqueza, rojo, rosa, amarillo, blanco, morado};

/**
 * PonerColor: Funcion para dar color al texto
 * @param c: color a dar
*/
string prjPonerColor(color c)
{
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c);
    return "";
}

/**
 * DividirCadena: Funcion para divir una cadena en un vector
 * @param str: cadena a dividir 
 * @param delim: caracter a dividir
*/
vector<string> prjDivirCadena(const string str, const char delim)
{
    vector<string> v;
    string strDiv;
    stringstream s(str);
    while (getline(s, strDiv, delim))
        v.push_back(strDiv);
    return v;
}

/**
 * validarNumero: Funcion para validar si un numero es positivo
 * @param numero: numero a validar
*/
bool prjValidarNumero(string numero)
{
    for (int i = 0; i < numero.length(); i++)
        if (!isdigit(numero[i]))
            return false;
    return true;
}

/**
 * ObtenerNumeroPositivo: Funcion para obtener un numero positivo
 * @param label: etiqueta del numero
 * @param numero: numero a obtener
*/
void prjObtenerNumeroPositivo(const string label, int &numero)
{
    string str;
    bool esNumerovalido = false;

    do
    {
        cout << label;
        cin >> str;
        esNumerovalido = prjValidarNumero(str);
        if(!esNumerovalido)
        {
            cout << "Entrada no valida. Introduzca un numero positivo" << endl;
        }
    }
    while(!esNumerovalido);

    numero = stoi(str);
}

/**
 * ValidarIdClientes: Funcion para validar si un id es unico
 * @param id: id a validar
 * @param clientes: vector de clientes
*/
bool prjValidarIDClientes(int id, const vector<Cliente> &clientes) 
{
    for (const auto &cliente : clientes) {
        if (cliente.ID == id) {
            return false;  
        }
    }
    return true;
}

/**
 * ValidarIDReservacion: Funcion para validar si un id es unico
 * @param id: id a validar
 * @param reservaciones: vector de reservaciones
*/
bool prjValidarIDReservacion(int id, const vector<Reservacion> &reservaciones)
{
    for (const auto &reservacion : reservaciones) 
    {
        if (reservacion.ID_Cliente == id) {
            return false;  
        }
    }
    return true;
}

/**
 * AgregarLetra: Funcion para agregar una letra
 * @param letra: letra a agregar
*/
void prjAgregarLetra(string &letra) 
{
    bool esLetraValida = false;

    do {
        cout << "Ingrese la letra N: ";
        cin >> letra;
        letra = toupper(letra[0]);

        if (letra == "N") {
            esLetraValida = true;
        } else {
            cout << "Entrada no válida. Ingrese solo la letra N." << endl;
        }
    } while (!esLetraValida);
}

/**
 * LeerReservaciones: Funcion para leer las reservaciones
 * @param pathFileName: nombre del archivo
*/
bool prjLeerClientes(string pathFileName)
{
    fstream f;
    string line;
    cout << "Leer archivo: " << pathFileName << endl;
    f.open(pathFileName, ios_base::in);
    if ( !f.is_open() )
    {
        cout << "Error al abrir el archivo: " << pathFileName << endl;
        return false;
    }
   else
    {
        clientes.clear();
        cout << "Abriendo Archivo: " << pathFileName << endl;
        getline(f, line);
        while (!f.eof())
        {
            getline(f, line);
            vector<string> vDatos = prjDivirCadena(line, ';');

            Cliente c;
            c.ID = stoi(vDatos.at(0));
            c.Cedula = vDatos.at(1);
            c.Nombre = vDatos.at(2);
            c.Direccion = vDatos.at(3);
            c.Asistencia = stoi(vDatos.at(4));
            clientes.push_back(c);
        }
        return true;
    }
    f.close();
    return false;
}

/**
 * MostrarClientes: Funcion para mostrar los clientes
*/
void prjMostrarClientes()
{
    cout << prjPonerColor(rosa) << endl
         << setw(4) << left << "ID"
         << setw(12) << left << "CEDULA"
         << setw(18) << left << "NOMBRE"
         << setw(12) << left << "DIRECCION"
         << setw(7) << left << "ASISTENCIA"
         << prjPonerColor(azul) << endl;

    for (const auto &cliente : clientes)
    {
        cout << setw(4) << left << cliente.ID
             << setw(12) << left << cliente.Cedula
             << setw(18) << left << cliente.Nombre
             << setw(12) << left << cliente.Direccion
             << setw(10) << left << cliente.Asistencia << endl;
    }
}

/**
 * AgregarCliente: Funcion para agregar un cliente
*/
void prjAgregarCliente()
{
    struct Cliente c;
    fflush(stdin);
    cout << endl;
    int id;
    do {
        prjObtenerNumeroPositivo("Ingrese ID Cliente: ", id);
        if (!prjValidarIDClientes(id, clientes)) 
        {
            cout << "El ID ya existe. Ingrese un ID único." << endl;
        }
    } while (!prjValidarIDClientes(id, clientes));

    c.ID = id;
    cin.ignore();
    do {
        cout << endl << "Ingrese Cedula del Cliente:  ";
        getline(cin, c.Cedula);
        if (c.Cedula.empty()) {
            cout << "La cedula no puede estar vacía. Ingrese nuevamente." << endl;
        }
    } while (c.Cedula.empty());

    do {
        cout << endl << "Ingrese Nombre del Cliente:  ";
        getline(cin, c.Nombre);
        if (c.Nombre.empty()) {
            cout << "El nombre no puede estar vacío. Ingrese nuevamente." << endl;
        }
    } while (c.Nombre.empty());

    do {
        cout << endl << "Ingrese Direccion del Cliente:  ";
        getline(cin, c.Direccion);
        if (c.Direccion.empty()) {
            cout << "La dirección no puede estar vacía. Ingrese nuevamente." << endl;
        }
    } while (c.Direccion.empty());

    cout << endl;
    prjObtenerNumeroPositivo("Ingrese Numero de Visitas al Hotel: ", c.Asistencia);
    cin.ignore();
    clientes.push_back(c); 
}

/**
 * GuardarClientes: Funcion para guardar los clientes
 * @param pathFileName: nombre del archivo
*/
void prjGuardarClientes(string pathFileName) 
{
    ofstream f(pathFileName);

    if (!f.is_open()) {
        cout << "Error al abrir el archivo: " << pathFileName << endl;
        return;
    }

    f << "ID;CEDULA;NOMBRE;DIRECCION;ASISTENCIA" << endl;
    int numClientes = clientes.size(); // Obtén el número total de clientes

    for (int i = 0; i < numClientes; i++) 
    {
        const auto &cliente = clientes[i];
        f << cliente.ID << ';' << cliente.Cedula << ';' << cliente.Nombre << ';' 
        << cliente.Direccion << ';' << cliente.Asistencia;
        if (i < numClientes - 1)
            f << '\n';
    }

    f.close();
}

/**
 * ExisteCliente: Funcion para validar si existe un cliente
 * @param ID: ID del cliente
*/
bool prjExisteCliente(int ID) 
{
    for (const auto &reservacion : reservaciones) {
        if (reservacion.ID_Cliente  == ID) {
            return true;
        }
    }
    return false;
}

/**
 * LeerReservaciones: Funcion para leer las reservaciones
 * @param pathFileName: nombre del archivo
*/
bool prjLeerReservaciones(string pathFileName)
{
    fstream f;
    string line;
    cout << "Leer archivo: " << pathFileName << endl;
    f.open(pathFileName, ios_base::in);
    if ( !f.is_open() )
    {
        cout << "Error al abrir el archivo: " << pathFileName << endl;
        return false;
    }
   else
    {
        reservaciones.clear();
        cout << "Abriendo Archivo: " << pathFileName << endl;
        getline(f, line);
        while (!f.eof())
        {
            getline(f, line);
            vector<string> vDatos = prjDivirCadena(line, ';');

            Reservacion r;
            r.ID_Cliente = stoi(vDatos.at(0));
            r.Nombre  = vDatos.at(1);
            r.Dias = stoi(vDatos.at(2));
            r.TAG = vDatos.at(3);
            r.Check = vDatos.at(4);
            reservaciones.push_back(r);
        }
        return true;
    }
    f.close();
    return false;
}

/**
 * MostrarReservaciones: Funcion para mostrar las reservaciones
*/
void prjMostrarReservaciones()
{
    cout << prjPonerColor(rosa) << endl
         << setw(11) << left << "ID_CLIENTE"
         << setw(18) << left << "NOMBRE"
         << setw(5) << left << "DIAS"
         << setw(18) << left << "TAG"
         << setw(15) << left << "CHECK"

         << prjPonerColor(azul) << endl;

    for (const auto &reservacion : reservaciones)
    {
        cout << setw(11) << left << reservacion.ID_Cliente
             << setw(18) << left << reservacion.Nombre
             << setw(5) << left << reservacion.Dias
             << setw(18) << left << reservacion.TAG
             << setw(15) << left << reservacion.Check << endl;
    }
}

/**
 * AgregarReservacion: Funcion para agregar una reservacion
*/
void prjAgregarReservacion()
{
    struct Reservacion r;
    fflush(stdin);
    cout << endl;
    int id;
    do {
        prjObtenerNumeroPositivo("Ingrese ID Cliente: ", id);
        if (!prjValidarIDReservacion(id, reservaciones)) 
        {
            cout << "El ID ya existe. Ingrese un ID único." << endl;
        }
    } while (!prjValidarIDReservacion(id, reservaciones));

    r.ID_Cliente = id;
    cin.ignore();
    do {
        cout << endl << "Ingrese Nombre del Cliente:  ";
        getline(cin, r.Nombre);
        if (r.Nombre.empty()) {
            cout << "El nombre no puede estar vacío. Ingrese nuevamente." << endl;
        }
    } while (r.Nombre.empty());

    cout << endl;
    prjObtenerNumeroPositivo("Ingrese Numero de Dias: ", r.Dias);
    cin.ignore();
    cout << endl << "Ingrese TAG:  ";
    prjAgregarLetra(r.TAG);
    cout << endl << "Ingrese CHECK:  ";
    prjAgregarLetra(r.Check);
    reservaciones.push_back(r);
}

/**
 * GuardarClientes: Funcion para guardar los clientes
 * @param pathFileName: nombre del archivo
*/
void prjGuardarReservaciones(string pathFileName) 
{
    ofstream f(pathFileName);

    if (!f.is_open()) {
        cout << "Error al abrir el archivo: " << pathFileName << endl;
        return;
    }

    f << "ID_CLIENTE;NOMBRE;DIAS;TAG;CHECK" << endl;
    int numReservaciones = reservaciones.size(); // Obtén el número total de reservaciones

    for (int i = 0; i < numReservaciones; i++) 
    {
        const auto &reservacion = reservaciones[i];
        f << reservacion.ID_Cliente << ';' << reservacion.Nombre << ';' 
          << reservacion.Dias << ';' << reservacion.TAG << ';' << reservacion.Check;
        if (i < numReservaciones - 1) 
            f << '\n';
    }
    f.close();
}

/**
 * LeerTag: Función para leer la línea que comienza con "UID" en el archivo NFC
 * @param ARCHIVOTAG: nombre del archivo
 */
string prjLeerTag(string ARCHIVOTAG)
{
    ifstream f(ARCHIVOTAG);
    string line;

    if (!f.is_open())
    {
        cout << "Error al abrir el archivo" << endl;
        return "";
    }

    while (getline(f, line))
    {
        if (line.find("UID") == 0) // Buscar línea que comienza con "UID"
        {
            f.close();
            return line;
        }
    }

    f.close();
    return "";
}

/**
 * HacerCheckIN: Funcion para cambiar el tag NFC y hacer CHECKIN
 * @param reservaciones: vector de reservaciones
 * @param ID: ID del cliente a cambiar TAG
*/
void prjHacerCheckIN(vector<Reservacion> &reservaciones) 
{
    int ID;
    bool clienteValido = false;

    do {
        prjObtenerNumeroPositivo("Ingrese el ID del cliente: ", ID);

        if (!prjExisteCliente(ID)) 
            cout << "El cliente con ID " << ID << " no existe." << endl;
         else 
            clienteValido = true;

    } while (!clienteValido);

    for (auto &reservacion : reservaciones) {
        if (reservacion.ID_Cliente == ID) {
            reservacion.TAG = prjLeerTag(ARCHIVOTAG);
            reservacion.Check = "Si";
        }
    }
}

/**
 *HacerCheckOUT: Funcion para cambiar el tag NFC y hacer CHECKOUT
 * @param reservaciones: vector de reservaciones
 * @param clientes: vector de clientes
 * @param ID: ID del cliente a cambiar TAG
*/
void prjHacerChekOUT(vector<Reservacion> &reservaciones, vector<Cliente> &clientes) 
{
    int ID;
    bool clienteValido = false;

    do {
        prjObtenerNumeroPositivo("Ingrese el ID del cliente: ", ID);

        if (!prjExisteCliente(ID)) {
            cout << "El cliente con ID " << ID << " no existe." << endl;
        } else {
            clienteValido = true;
        }

    } while (!clienteValido);

    for (auto it = reservaciones.begin(); it != reservaciones.end(); ) {
        if (it->ID_Cliente == ID && it->Check == "Si" && it->TAG == prjLeerTag(ARCHIVOTAG)) 
        {
            it = reservaciones.erase(it); // Elimina el elemento y mueve el iterador al siguiente

            for (auto &cliente : clientes) 
            {
                if (cliente.ID == ID)
                    cliente.Asistencia++;
            }
            prjGuardarClientes(ARCHIVOCLIENTESGENERAL);
        } else {
            cout << "No se pudo realizar el checkout porque no hizo check in o el TAG no coincide" << endl;
            ++it; // Avanza al siguiente elemento
        }
    }
    prjGuardarReservaciones(ARCHIVORESERVACIONES);
}

/**
 * MenuPrincipal: Funcion para mostrar el menu principal
 * @param opc: opcion elegida
*/
void prjMenuPrincipal(int &opc)
{
    bool opcValida = false;
    do
    {
        cout << endl << prjPonerColor(rojo) << "\t\t\tMenu Principal" << endl;
        cout << endl << prjPonerColor(azul) << "\t1. Clientes Nuevo" << endl
                                            << "\t2. Cliente Antiguos" << endl
                                            << "\t3. Salir" << endl;
        cout << "\tIngresar Opcion: ";
        try
        {
            cin >> opc;
            cin.clear();
            if (opc>0 && opc<=3)
            {
                opcValida = true;
                break;
            }
            throw invalid_argument("Opcion invalida");
        }
        catch(...)
        {
            cout << "Opcion invalida" << endl;
            fflush(stdin);
        }
    } while (!opcValida);
}

/**
 * MenuClientesNuevos: Funcion para mostrar el menu de clientes nuevos
 * @param opc: opcion elegida
*/
void prjMenuClientesNuevos(int &opc)
{
    bool opcValida = false;
    do
    {
        cout << endl << prjPonerColor(rojo) << "\t\t\tMenu Clientes Nuevos" << endl;
        cout << endl << prjPonerColor(azul) << "\t1. Ingresar Cliente" << endl
                                            << "\t2. Guardar Cliente" << endl
                                            << "\t3. Salir al Menu principal" << endl;
        cout << "\tIngresar Opcion: ";
        try
        {
            cin >> opc;
            cin.clear();
            if (opc>0 && opc<=3)
            {
                opcValida = true;
                break;
            }
            throw invalid_argument("Opcion invalida");
        }
        catch(...)
        {
            cout << "Opcion invalida" << endl;
            fflush(stdin);
        }
    } while (!opcValida);
}

/**
 * MenuClientesAntiguos: Funcion para mostrar el menu de clientes antiguos
 * @param opc: opcion elegida
*/
void prjMenuClientesAntiguos(int &opc)
{
    bool opcValida = false;
    do
    {
        cout << endl << prjPonerColor(rojo) << "\t\t\tMenu Reservaciones" << endl;
        cout << endl << prjPonerColor(azul) << "\t1. Agregar Reservacion" << endl
                                            << "\t2. CheckIN" << endl
                                            << "\t3. CheckOUT" << endl
                                            << "\t4. Salir al menu principal" << endl;
        cout << "\tIngresar Opcion: ";
        try
        {
            cin >> opc;
            cin.clear();
            if (opc>0 && opc<=4)
            {
                opcValida = true;
                break;
            }
            throw invalid_argument("Opcion invalida");
        }
        catch(...)
        {
            cout << "Opcion invalida" << endl;
            fflush(stdin);
        }
    } while (!opcValida);
}

int main ()
{
    cout << LIMPIAR << endl;
    int eleccion = 0;
    bool salir = false, salirClienteNuevo = false, salirClienteAntiguo= false;
    fflush(stdin);
    do
    {
        prjLeerClientes(ARCHIVOCLIENTESGENERAL);
        prjLeerReservaciones(ARCHIVORESERVACIONES);
        cout << LIMPIAR;
        cout << endl << prjPonerColor(blanco) << "\t\tBienvenido al Sistema de Reservaciones" << endl;
        cout << "\t   Sistema de reservaciones del hotel \"La Paz\"" << endl;
        prjMostrarClientes();
        prjMostrarReservaciones();
        prjMenuPrincipal(eleccion);
        switch (eleccion)
        {
            case 1:
            {
                int OpcMenuClientesNuevos = 0;
                do
                {
                    cout << LIMPIAR;
                    cout << endl << prjPonerColor(blanco) << "\t\t\tBienvenido al Sistema de Reservaciones" << endl;
                    cout << "\tSistema de reservaciones del hotel \"La Paz\"" << endl;
                    prjMenuClientesNuevos(OpcMenuClientesNuevos);
                    switch (OpcMenuClientesNuevos)
                    {
                        case 1: 
                        {   
                            prjMostrarClientes();
                            prjAgregarCliente();
                                break;
                        }
                        case 2: 
                        {   
                            prjGuardarClientes(ARCHIVOCLIENTESGENERAL);
                                break;
                        }
                        case 3: salirClienteNuevo = true;
                                break;
                    }
                } while (!salirClienteNuevo);
                salirClienteNuevo = false;
                break;
            } 
            case 2:
            {
                int OpcReservaciones = 0;
                do
                {
                    cout << LIMPIAR;
                    prjMostrarReservaciones();
                    prjMenuClientesAntiguos(OpcReservaciones);
                    switch (OpcReservaciones)
                    {
                        case 1: 
                        {   
                            prjMostrarClientes();
                            prjAgregarReservacion();
                            prjGuardarReservaciones(ARCHIVORESERVACIONES);
                                break;
                        }
                        case 2: 
                        {
                            prjHacerCheckIN(reservaciones);
                            prjGuardarReservaciones(ARCHIVORESERVACIONES);
                                break;
                        }                            
                        case 3: 
                        {
                            prjHacerChekOUT(reservaciones, clientes);
                                break;
                        }
                        case 4: salirClienteAntiguo = true;
                                break;  
                    }
                } while (!salirClienteAntiguo);
                salirClienteAntiguo = false;
                break;
            }
            case 3:
            {
                salir = true;
                break;
            }
        }
    } while (!salir);
    return 0;
}