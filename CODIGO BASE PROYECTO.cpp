#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

#ifdef _WIN32
#include <windows.h> // Para UTF-8 en consola Windows (opcional)
#endif

using namespace std;

// ===================== ESTRUCTURAS =====================

struct Direccion {
    string provincia;
    string ciudad;
    string callePrincipal;
    string calleSecundaria;
};

struct Persona {
    string tipoId;      // "CEDULA", "RUC", "PASAPORTE"
    string numeroId;
    string nombreApellido;
    string telefono;
    string provincia;
    string ciudad;
    string callePrincipal;
    string calleSecundaria;
};

struct Factura {
    string idFactura;   // 10 digitos (mismo esquema del paquete si desea)
    string tipoId;
    string numeroId;
    string nombreApellido;
    string telefono;
    string provincia;
    string ciudad;
    string callePrincipal;
    string calleSecundaria;
};

struct EntregaFinal {
    string tipoId;
    string numeroId;
    string nombreApellido;
};

struct Paquete {
    string idPaquete;             // 10 digitos (string para no perder ceros si aplica)
    Persona enviador;
    Persona receptor;
    Direccion destino;
    string contenido;
    float pesoKg;
    int cantidadObjetos;

    Factura factura;

    bool entregado;               // estado
    EntregaFinal recibidoPor;     // solo si entregado

    vector<string> ubicaciones;   // HISTORIAL SOLO EN MEMORIA (NO se guarda en archivo)
};

// ===================== UTILIDADES =====================

string trim(const string &s) {
    size_t i = 0, j = s.size();
    while (i < j && (s[i] == ' ' || s[i] == '\t' || s[i] == '\r' || s[i] == '\n')) i++;
    while (j > i && (s[j-1] == ' ' || s[j-1] == '\t' || s[j-1] == '\r' || s[j-1] == '\n')) j--;
    return s.substr(i, j - i);
}

// Lee una línea completa (evita problemas mezclando cin >> y getline)
string leerLinea(const string &msg) {
    cout << msg;
    string x;
    getline(cin, x);
    return trim(x);
}

int leerEntero(const string &msg, int minV, int maxV) {
    while (true) {
        cout << msg;
        string s;
        getline(cin, s);
        stringstream ss(s);
        int v;
        if (ss >> v && v >= minV && v <= maxV) return v;
        cout << "Valor invalido. Intente de nuevo.\n";
    }
}

float leerFloat(const string &msg, float minV) {
    while (true) {
        cout << msg;
        string s;
        getline(cin, s);
        stringstream ss(s);
        float v;
        if (ss >> v && v >= minV) return v;
        cout << "Valor invalido. Intente de nuevo.\n";
    }
}

string seleccionarTipoId() {
    while (true) {
        cout << "\nTipo de identificacion:\n";
        cout << "1) CEDULA\n2) RUC\n3) PASAPORTE\nOpcion: ";
        string op;
        getline(cin, op);
        if (op == "1") return "CEDULA";
        if (op == "2") return "RUC";
        if (op == "3") return "PASAPORTE";
        cout << "Opcion invalida.\n";
    }
}

// ===================== PERSISTENCIA (TXT) =====================
// Guardamos en "paquetes.txt" con separador "||" (evita conflictos con tildes).
// NO guardamos ubicaciones (solo memoria) según su requisito.

const string ARCH_PAQ = "paquetes.txt";
const string ARCH_ID  = "id_counter.txt";

string leerProximoId() {
    ifstream f(ARCH_ID);
    long long id = 2134000000LL;         // inicio solicitado
    if (f.good()) {
        f >> id;
        if (!f) id = 2134000000LL;
    }
    // guardamos el siguiente +1
    ofstream out(ARCH_ID);
    out << (id + 1);
    // retornamos el actual como string
    return to_string(id);
}

string safeField(const string &x) {
    // Evita romper el formato si el usuario escribe "||"
    string r = x;
    // Reemplaza "||" por "| |" (simple)
    size_t pos = 0;
    while ((pos = r.find("||", pos)) != string::npos) {
        r.replace(pos, 2, "| |");
        pos += 3;
    }
    return r;
}

string serializePersona(const Persona &p) {
    // Campos en orden fijo
    // tipoId||numeroId||nombreApellido||telefono||provincia||ciudad||callePrincipal||calleSecundaria
    ostringstream os;
    os << safeField(p.tipoId) << "||" << safeField(p.numeroId) << "||"
       << safeField(p.nombreApellido) << "||" << safeField(p.telefono) << "||"
       << safeField(p.provincia) << "||" << safeField(p.ciudad) << "||"
       << safeField(p.callePrincipal) << "||" << safeField(p.calleSecundaria);
    return os.str();
}

bool deserializePersona(const vector<string> &v, int start, Persona &p) {
    // necesita 8 campos
    if ((int)v.size() < start + 8) return false;
    p.tipoId = v[start+0];
    p.numeroId = v[start+1];
    p.nombreApellido = v[start+2];
    p.telefono = v[start+3];
    p.provincia = v[start+4];
    p.ciudad = v[start+5];
    p.callePrincipal = v[start+6];
    p.calleSecundaria = v[start+7];
    return true;
}

string serializeFactura(const Factura &f) {
    ostringstream os;
    os << safeField(f.idFactura) << "||" << safeField(f.tipoId) << "||" << safeField(f.numeroId) << "||"
       << safeField(f.nombreApellido) << "||" << safeField(f.telefono) << "||"
       << safeField(f.provincia) << "||" << safeField(f.ciudad) << "||"
       << safeField(f.callePrincipal) << "||" << safeField(f.calleSecundaria);
    return os.str();
}

bool deserializeFactura(const vector<string> &v, int start, Factura &f) {
    // necesita 9 campos
    if ((int)v.size() < start + 9) return false;
    f.idFactura = v[start+0];
    f.tipoId = v[start+1];
    f.numeroId = v[start+2];
    f.nombreApellido = v[start+3];
    f.telefono = v[start+4];
    f.provincia = v[start+5];
    f.ciudad = v[start+6];
    f.callePrincipal = v[start+7];
    f.calleSecundaria = v[start+8];
    return true;
}

vector<string> splitBy(const string &s, const string &delim) {
    vector<string> out;
    size_t start = 0;
    while (true) {
        size_t pos = s.find(delim, start);
        if (pos == string::npos) {
            out.push_back(s.substr(start));
            break;
        }
        out.push_back(s.substr(start, pos - start));
        start = pos + delim.size();
    }
    return out;
}

string serializePaquete(const Paquete &p) {
    // Orden:
    // idPaquete ||
    // Persona enviador (8) ||
    // Persona receptor (8) ||
    // destino (4) ||
    // contenido || peso || cantidad ||
    // factura (9) ||
    // entregado || recibidoPorTipoId || recibidoPorNumeroId || recibidoPorNombre
    ostringstream os;
    os << safeField(p.idPaquete) << "||"
       << serializePersona(p.enviador) << "||"
       << serializePersona(p.receptor) << "||"
       << safeField(p.destino.provincia) << "||" << safeField(p.destino.ciudad) << "||"
       << safeField(p.destino.callePrincipal) << "||" << safeField(p.destino.calleSecundaria) << "||"
       << safeField(p.contenido) << "||"
       << p.pesoKg << "||"
       << p.cantidadObjetos << "||"
       << serializeFactura(p.factura) << "||"
       << (p.entregado ? 1 : 0) << "||"
       << safeField(p.recibidoPor.tipoId) << "||"
       << safeField(p.recibidoPor.numeroId) << "||"
       << safeField(p.recibidoPor.nombreApellido);
    return os.str();
}

bool deserializePaquete(const string &linea, Paquete &p) {
    vector<string> v = splitBy(linea, "||");

    // Debe tener mínimo los campos esperados
    // id(1) + enviador(8) + receptor(8) + destino(4) + contenido(1) + peso(1) + cant(1) + factura(9) + estado(1)+rec(3)
    // total = 1+8+8+4+1+1+1+9+1+3 = 37
    if ((int)v.size() < 37) return false;

    int idx = 0;
    p.idPaquete = v[idx++];

    if (!deserializePersona(v, idx, p.enviador)) return false;
    idx += 8;

    if (!deserializePersona(v, idx, p.receptor)) return false;
    idx += 8;

    p.destino.provincia = v[idx++];
    p.destino.ciudad = v[idx++];
    p.destino.callePrincipal = v[idx++];
    p.destino.calleSecundaria = v[idx++];

    p.contenido = v[idx++];

    // peso
    try { p.pesoKg = stof(v[idx++]); } catch (...) { return false; }
    // cantidad
    try { p.cantidadObjetos = stoi(v[idx++]); } catch (...) { return false; }

    if (!deserializeFactura(v, idx, p.factura)) return false;
    idx += 9;

    // entregado
    p.entregado = (v[idx++] == "1");

    p.recibidoPor.tipoId = v[idx++];
    p.recibidoPor.numeroId = v[idx++];
    p.recibidoPor.nombreApellido = v[idx++];

    p.ubicaciones.clear(); // No se carga historial desde archivo (requisito)
    return true;
}

void guardarTodo(const vector<Paquete> &paquetes) {
    ofstream f(ARCH_PAQ, ios::out | ios::trunc);
    for (int i = 0; i < (int)paquetes.size(); i++) {
        f << serializePaquete(paquetes[i]) << "\n";
    }
}

void cargarTodo(vector<Paquete> &paquetes) {
    paquetes.clear();
    ifstream f(ARCH_PAQ);
    string line;
    while (getline(f, line)) {
        if (trim(line).empty()) continue;
        Paquete p;
        if (deserializePaquete(line, p)) paquetes.push_back(p);
    }
}

// ===================== BÚSQUEDA =====================

int buscarPaquetePorId(const vector<Paquete> &paquetes, const string &id) {
    for (int i = 0; i < (int)paquetes.size(); i++) {
        if (paquetes[i].idPaquete == id) return i;
    }
    return -1;
}

// ===================== CAPTURA DE DATOS =====================

Persona ingresarPersona(const string &titulo) {
    cout << "\n--- " << titulo << " ---\n";
    Persona p;
    p.tipoId = seleccionarTipoId();
    p.numeroId = leerLinea("Numero de identificacion: ");
    p.nombreApellido = leerLinea("Nombre y apellido: ");
    p.telefono = leerLinea("Telefono: ");
    p.provincia = leerLinea("Provincia: ");
    p.ciudad = leerLinea("Ciudad: ");
    p.callePrincipal = leerLinea("Calle principal: ");
    p.calleSecundaria = leerLinea("Calle secundaria: ");
    return p;
}

Factura ingresarFactura(const string &idFacturaSugerido) {
    cout << "\n--- DATOS DE FACTURA ---\n";
    Factura f;
    f.idFactura = idFacturaSugerido; // asignación automática solicitada
    cout << "ID Factura (auto): " << f.idFactura << "\n";
    f.tipoId = seleccionarTipoId();
    f.numeroId = leerLinea("Numero de identificacion (factura): ");
    f.nombreApellido = leerLinea("Nombre y apellido (factura): ");
    f.telefono = leerLinea("Telefono (factura): ");
    f.provincia = leerLinea("Provincia (factura): ");
    f.ciudad = leerLinea("Ciudad (factura): ");
    f.callePrincipal = leerLinea("Calle principal (factura): ");
    f.calleSecundaria = leerLinea("Calle secundaria (factura): ");
    return f;
}

// ===================== MOSTRAR =====================

void mostrarPaquete(const Paquete &p, bool modoCliente) {
    cout << "\n========== PAQUETE ==========\n";
    cout << "ID Paquete: " << p.idPaquete << "\n";
    cout << "Estado: " << (p.entregado ? "ENTREGADO" : "NO ENTREGADO") << "\n";

    cout << "\n--- ENVIADOR ---\n";
    cout << "Tipo ID: " << p.enviador.tipoId << " | Numero: " << p.enviador.numeroId << "\n";
    cout << "Nombre: " << p.enviador.nombreApellido << "\n";
    cout << "Telefono: " << p.enviador.telefono << "\n";
    cout << "Direccion: " << p.enviador.provincia << "-" << p.enviador.ciudad
         << " / " << p.enviador.callePrincipal << " y " << p.enviador.calleSecundaria << "\n";

    cout << "\n--- RECEPTOR ---\n";
    cout << "Tipo ID: " << p.receptor.tipoId << " | Numero: " << p.receptor.numeroId << "\n";
    cout << "Nombre: " << p.receptor.nombreApellido << "\n";
    cout << "Telefono: " << p.receptor.telefono << "\n";
    cout << "Direccion: " << p.receptor.provincia << "-" << p.receptor.ciudad
         << " / " << p.receptor.callePrincipal << " y " << p.receptor.calleSecundaria << "\n";

    cout << "\n--- DESTINO (ENVIO) ---\n";
    cout << "Provincia/Ciudad: " << p.destino.provincia << "-" << p.destino.ciudad << "\n";
    cout << "Calles: " << p.destino.callePrincipal << " y " << p.destino.calleSecundaria << "\n";

    cout << "\n--- PAQUETE ---\n";
    cout << "Contenido: " << p.contenido << "\n";
    cout << "Peso (kg): " << p.pesoKg << "\n";
    cout << "Cantidad de objetos: " << p.cantidadObjetos << "\n";

    cout << "\n--- FACTURA ---\n";
    cout << "ID Factura: " << p.factura.idFactura << "\n";
    cout << "Tipo ID: " << p.factura.tipoId << " | Numero: " << p.factura.numeroId << "\n";
    cout << "Nombre: " << p.factura.nombreApellido << "\n";
    cout << "Telefono: " << p.factura.telefono << "\n";
    cout << "Direccion: " << p.factura.provincia << "-" << p.factura.ciudad
         << " / " << p.factura.callePrincipal << " y " << p.factura.calleSecundaria << "\n";

    cout << "\n--- UBICACIONES (historial en memoria) ---\n";
    if (p.ubicaciones.empty()) {
        cout << "(Sin registros de ubicacion)\n";
    } else {
        for (int i = 0; i < (int)p.ubicaciones.size(); i++) {
            cout << i+1 << ") " << p.ubicaciones[i] << "\n";
        }
    }

    if (p.entregado) {
        cout << "\n--- ENTREGADO A ---\n";
        cout << "Tipo ID: " << p.recibidoPor.tipoId << " | Numero: " << p.recibidoPor.numeroId << "\n";
        cout << "Nombre: " << p.recibidoPor.nombreApellido << "\n";
    }

    // Cliente NO altera nada: solo ve
    if (modoCliente) {
        cout << "\n(MODO CLIENTE: Solo lectura)\n";
    }
}

// ===================== ADMIN: OPERACIONES =====================

void adminCrearPaquete(vector<Paquete> &paquetes) {
    Paquete p;

    p.idPaquete = leerProximoId();         // ID incremental en archivo
    cout << "\nID de Paquete asignado: " << p.idPaquete << "\n";

    p.enviador = ingresarPersona("DATOS DEL ENVIADOR");
    p.receptor = ingresarPersona("DATOS DEL RECEPTOR");

    cout << "\n--- DESTINO ---\n";
    p.destino.provincia = leerLinea("Provincia destino: ");
    p.destino.ciudad = leerLinea("Ciudad destino: ");
    p.destino.callePrincipal = leerLinea("Calle principal destino: ");
    p.destino.calleSecundaria = leerLinea("Calle secundaria destino: ");

    cout << "\n--- DETALLE DEL PAQUETE ---\n";
    p.contenido = leerLinea("Que lleva el paquete: ");
    p.pesoKg = leerFloat("Peso del paquete (kg): ", 0.01f);
    p.cantidadObjetos = leerEntero("Cantidad de objetos: ", 1, 1000000);

    // Factura: usa el mismo esquema ID incremental (si quiere otro contador, se separa)
    p.factura = ingresarFactura(p.idPaquete);

    p.entregado = false;
    p.recibidoPor = {"", "", ""};
    p.ubicaciones.clear();

    paquetes.push_back(p);
    guardarTodo(paquetes); // Persistir (sin ubicaciones)
    cout << "\nPaquete creado y guardado en archivo.\n";
}

void adminVerPaquetes(vector<Paquete> &paquetes) {
    if (paquetes.empty()) {
        cout << "No hay paquetes registrados.\n";
        return;
    }

    cout << "\n1) Buscar por ID\n2) Ver todos\nOpcion: ";
    string op;
    getline(cin, op);

    if (op == "1") {
        string id = leerLinea("Ingrese ID de paquete: ");
        int pos = buscarPaquetePorId(paquetes, id);
        if (pos == -1) {
            cout << "Paquete no encontrado.\n";
            return;
        }

        mostrarPaquete(paquetes[pos], false);

        // opción extra de ubicación en admin
        cout << "\nDesea agregar ubicacion actual? (1=SI / 0=NO): ";
        string x;
        getline(cin, x);
        if (x == "1") {
            string prov = leerLinea("Provincia ubicacion: ");
            string ciu = leerLinea("Ciudad ubicacion: ");
            paquetes[pos].ubicaciones.push_back(prov + "-" + ciu);
            cout << "Ubicacion agregada en memoria.\n";
        }
    }
    else if (op == "2") {
        for (int i = 0; i < (int)paquetes.size(); i++) {
            cout << "\n[" << (i+1) << "/" << paquetes.size() << "]";
            mostrarPaquete(paquetes[i], false);
        }
    }
    else {
        cout << "Opcion invalida.\n";
    }
}

void adminRegistrarEntrega(vector<Paquete> &paquetes) {
    string id = leerLinea("Ingrese ID del paquete a ENTREGAR: ");
    int pos = buscarPaquetePorId(paquetes, id);
    if (pos == -1) {
        cout << "Paquete no encontrado.\n";
        return;
    }

    if (paquetes[pos].entregado) {
        cout << "Este paquete ya esta marcado como ENTREGADO.\n";
        return;
    }

    cout << "\n--- DATOS DE QUIEN RECIBE ---\n";
    EntregaFinal ef;
    ef.tipoId = seleccionarTipoId();
    ef.numeroId = leerLinea("Numero de identificacion: ");
    ef.nombreApellido = leerLinea("Nombre y apellido: ");

    paquetes[pos].recibidoPor = ef;
    paquetes[pos].entregado = true;

    guardarTodo(paquetes); // Se guarda estado entregado y datos del receptor final
    cout << "Entrega registrada. Paquete marcado como ENTREGADO.\n";
}

// ===================== CLIENTE =====================

void clienteBuscarPaquete(const vector<Paquete> &paquetes) {
    string id = leerLinea("Ingrese ID de paquete: ");
    int pos = buscarPaquetePorId(paquetes, id);
    if (pos == -1) {
        cout << "Paquete no encontrado.\n";
        return;
    }
    mostrarPaquete(paquetes[pos], true);
}

// ===================== LOGIN ADMIN =====================

bool loginAdmin() {
    // Credenciales simples (base). En Qt puede validarlo con DB/archivo.
    const string USER = "Nicolay";
    const string PASS = "Barreno";

    cout << "\n--- LOGIN ADMINISTRADOR ---\n";
    string u = leerLinea("Usuario: ");
    string p = leerLinea("Contrasena: ");

    if (u == USER && p == PASS) return true;
    cout << "Credenciales incorrectas.\n";
    return false;
}

// ===================== MENÚS =====================

void menuAdministrador(vector<Paquete> &paquetes) {
    if (!loginAdmin()) return;

    while (true) {
        cout << "\n===== POLI-ENTREGAS | ADMIN =====\n";
        cout << "1) Crear Paquete\n";
        cout << "2) Ver / Buscar Paquetes (y agregar ubicacion)\n";
        cout << "3) Registrar Entrega (marcar entregado)\n";
        cout << "0) Volver\n";
        int op = leerEntero("Opcion: ", 0, 3);

        if (op == 0) break;
        if (op == 1) adminCrearPaquete(paquetes);
        if (op == 2) adminVerPaquetes(paquetes);
        if (op == 3) adminRegistrarEntrega(paquetes);
    }
}

void menuCliente(const vector<Paquete> &paquetes) {
    cout << "\n===== POLI-ENTREGAS | CLIENTE =====\n";
    cout << "Busqueda por ID (solo lectura)\n";
    clienteBuscarPaquete(paquetes);
}

// ===================== MAIN =====================

int main() {
#ifdef _WIN32
    // Esto ayuda a que la consola muestre tildes/ñ si el entorno lo permite.
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    vector<Paquete> paquetes;
    cargarTodo(paquetes); // carga paquetes desde txt (sin ubicaciones)

    while (true) {
        cout << "\n========== POLI-ENTREGAS ==========\n";
        cout << "1) Administrador (con login)\n";
        cout << "2) Cliente\n";
        cout << "0) Salir\n";
        int op = leerEntero("Opcion: ", 0, 2);

        if (op == 0) break;
        if (op == 1) menuAdministrador(paquetes);
        if (op == 2) menuCliente(paquetes);

        // Re-cargar desde archivo por si se actualizó estado/paquetes
        cargarTodo(paquetes);
    }

    cout << "Saliendo...\n";
    return 0;
}
