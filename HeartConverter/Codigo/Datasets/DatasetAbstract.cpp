/**
 * @file DatasetAbstract.cpp
 * 
 * Clase abstracta que sigue el patrón de diseño "multiton". Esta clase estática
 * almacena los punteros a toda la información disponible que se haya extraido
 * del fichero de entrada y proporciona un acceso único a estos.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#include "DatasetAbstract.h"
#include "Dataset.h"
#include <unordered_map>
#include <string>
#include <iostream>
using namespace std;

unordered_map<string, DatasetAbstract*> DatasetAbstract::dataset_names;

/**
 * Constructor. Inicializa el atributo name del dataset, con el que luego podrá
 * ser accedido. Además llama a la función que crea este punto de acceso.
 * Este constructor solo debe de ser llamado a traves del método factoría.
 * 
 * @param [in]  name    Nombre y llave del dataset/conj. de datos.
 * @param [in]  size    Cantidad de elementos a reservar. Evitamos tener que reasignar
 *                      memoria.
 */
DatasetAbstract::DatasetAbstract(const string& name, unsigned int size = 0) {
    this->name = name;
    addDataset(name);
}

/**
 * Genera un puntero que apunta al propio objeto que a invocado este método y 
 * lo añade a una tabla hash para poder ser accedido posteriormente.
 * 
 * @param [in]  name    Nombre y llave que identifica al conj. de datos en la 
 *                      tabla hash.
 **/
void DatasetAbstract::addDataset (const string& name) {
    DatasetAbstract* pointer (this);    
    dataset_names.emplace(name, pointer);
}

/**
 * Dado un nombre la función trata de buscar el conjunto de datos con la llave
 * correspondiende.
 * 
 * @param [in]  name    Nombre y llave del conj. de datos del que queremos acceso.
 * @return Devuelve un puntero al conj. de datos si lo encuentra, nullptr en
 *         caso contrario.
 **/
DatasetAbstract*  DatasetAbstract::getDataset(const std::string& name) {
    DatasetAbstract* result;
    
    auto search = dataset_names.find(name);
    if (search != dataset_names.end())
        result = search->second;
    else {
        cout << "No se ha encontrado el conjunto de datos llamado " << name << endl;
        result = nullptr;
    }
    
    return result;
}


/**
 * Devuelve el nombre del conjunto de datos que lo invoca.
 **/
string DatasetAbstract::getName() {
    return name;
}

/**
 * Método factoría con el que se crean las instancias concretas (no abstractas)
 * de todas los conj. de datos que sean necesarios y con las características
 * deseadas.
 * 
 * @param [in]  type    El tipo de dato en el que almacenara la información
 *                      internamente (float, unsigned int, etc)
 * @param [in]  name    Nombre y llave que identifica al conj. de datos en la
 *                      tabla hash.
 **/
DatasetAbstract* DatasetAbstract::FactoryDataset (const std::string& type, const std::string& name, unsigned int size = 0) {
    DatasetAbstract* pointer = nullptr;
    
    if (type == "bool")
        pointer = new Dataset<bool>(name, size);
    
    else if (type == "char")
        pointer = new Dataset<char>(name, size);
    else if (type == "unsigned char")
        pointer = new Dataset<unsigned char>(name, size);
    else if (type == "signed char")
        pointer = new Dataset<signed char>(name, size);
    
    else if (type == "short")
        pointer = new Dataset<short>(name, size);
    else if (type == "unsigned short")
        pointer = new Dataset<unsigned short>(name, size);
    else if (type == "signed short")
        pointer = new Dataset<signed short>(name, size);
    
    else if (type == "int")
        pointer = new Dataset<int>(name, size);
    else if (type == "unsigned int")
        pointer = new Dataset<unsigned int>(name, size);
    else if (type == "signed int")
        pointer = new Dataset<signed int>(name, size);
    
    else if (type == "float")
        pointer = new Dataset<float>(name, size);
    else if (type == "double")
        pointer = new Dataset<double>(name, size);
    
    /*else if (type == "string")
        pointer = new Dataset<string>(name, size);*/
    
    else if (type == "long long")
        pointer = new Dataset<long long>(name, size);
    else if (type == "unsigned long long")
        pointer = new Dataset<unsigned long long>(name, size);
    else if (type == "signed long long")
        pointer = new Dataset<signed long long>(name, size);
    
    else if (type == "int64_t")
        pointer = new Dataset<int64_t>(name, size);
    else if (type == "uint64_t")
        pointer = new Dataset<uint64_t>(name, size);
    
    else
        cout << type << " data type not supported" << endl;

    return pointer;
}

DatasetAbstract::~DatasetAbstract() {
}

