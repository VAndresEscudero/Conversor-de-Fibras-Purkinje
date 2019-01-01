/**
 * @file CarpPurkinje.cpp
 * 
 * Clase que representa un fichero de fibras de Purkinje en CARP.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#include "CarpPurkinje.h"
#include <vector>
#include <array>
#include <iostream>
#include <map>
#include <utility>
#include <fstream>
#include <exception>
#include <algorithm>
#include <functional>
using namespace std;

/**
 * Constructor. Llama al constructor de la clase de la que hereda para
 * inicializar los valores del nombre y extensión del fichero (.pkje). También
 * inicializa los punteros a la información necesaria y el vector donde se
 * guardan las relaciones de padres e hijos entre fibras de Purkinje. Finalmente
 * llama a las funciones requeridas que calculan el resto de datos necesarios
 * para producir el fichero, como el fichero de configuración o las relaciones
 * entre fibras.
 * 
 * @param [in]  name    Nombre del fichero.
 **/
CarpPurkinje::CarpPurkinje(const std::string& name) : AbstractFile(name, ".pkje") {
    
    points = DatasetAbstract::getDataset("points");
    elements = DatasetAbstract::getDataset("elements");
    
    setAttributesValues();
    calcRelations();
    removeExtraRelations();
    
    printSeveralParents();

}

void CarpPurkinje::printSeveralParents() {
    
    for (auto it = searchParents.begin(); it != searchParents.end(); ) {
        unsigned int number_relations = 0;
        
        auto range = searchSons.equal_range(it->first);
        for (auto local_it = range.first; local_it != range.second; ++local_it){
            if (number_relations >= MAX_SONS){
                cout << "Mas de 2 padres: " << it->second << endl;
            }
            
            ++number_relations;
            
        }
        
        it = range.second;
    }
}

/**
 * Función que compureba que el fichero de configuración exista o lo crea en 
 * caso contrario. Tras esto asigna los valores del fichero a los parámetros de
 * la clase.
 **/
void CarpPurkinje::setAttributesValues() {
    ifstream file(config_name);
    
    if(!file.good()){
        file.close();
        createDefaultConfigFile();
        file.open(config_name);
    }
    
    string line;
    
    getline(file, line);
    try {
        cable_size = stoi(line);
    } catch (...) {}
    
    getline(file, line);
    try {
        gap_resistance = stof(line);
    } catch (...) {}
    
    
    getline(file, line);
    try {
        conductivity = stof(line);
    } catch (...) {}
}

/**
 * Crea un fichero de configuración estandar. Este indica al programa que
 * valores usar para las variables de tamaño, resistencia y conductividad de las 
 * fibras de Purkinje.
 **/
void CarpPurkinje::createDefaultConfigFile() {
    ofstream file(config_name);
    
    file << cable_size << " #Tamano relativo del cable. Numero de fibras paralelas o area transversal.\n";
    
    file << gap_resistance << " #Resistencia en kOmh de las uniones gap.\n";
    
    file << conductivity << " #Conductividad en Omh por cm de las fibras.\n";
    
    file << "         #El orden de las lineas es importante.\n" << flush;
    file.close();
}

/**
 * Función que itera sobra cada una de las fibras de Purkinje y populando sus
 * relaciones con las otras fibras.
 **/
void CarpPurkinje::calcRelations() {
    
    vector<double> fiber;
    vector<double> coords_beg, coords_end;
    size_t last_index;
    
    //size_t number_of_elements = elements->size();
    for (size_t i = 0; i < elements->size(); ++i) {
        elements->getData(i, fiber);
        last_index = fiber.size() - 1;
        
        points->getData(fiber[0], coords_beg);
        points->getData(fiber[last_index], coords_end);
        
        searchParents.insert(pair< vector<double>, unsigned int>(coords_end, i));
        searchSons.insert(pair< vector<double>, unsigned int>( coords_beg, i));
    }
}

void CarpPurkinje::removeExtraRelations () {

start_loop:   
    for (auto it = searchSons.begin(); it != searchSons.end(); ) {
        unsigned int number_relations = 0;
        
        auto range = searchSons.equal_range(it->first);
        for (auto local_it = range.first; local_it != range.second; ++local_it){
            if (number_relations >= MAX_SONS){
                removeExtraSon(local_it->first, local_it->second);
                goto start_loop;  
            }
            
            ++number_relations;
            
        }
        
        it = range.second;
    }
    
}

void CarpPurkinje::removeExtraSon(vector<double> point, unsigned int cable_id){
    //Buscamos el primer elemento que sea padre del hijo
    vector<double> cable_to_divide, point_cable;
    size_t penultimate_point_index;
    
    //Lo buscamos en la otra tabla
    auto search = searchParents.find(point);
    if (search != searchParents.end()){
        
        unsigned int parent_index = search->second;
        
        elements->getData(parent_index, cable_to_divide);
        penultimate_point_index = cable_to_divide.size() - 2;
        points->getData(cable_to_divide[penultimate_point_index], point_cable);
        
        size_t new_point_index = createNewPoint(point_cable, point, 0.9);
        
        size_t new_cable_index = createNewElement(parent_index, cable_id, new_point_index);
        
        addRelations(parent_index,new_cable_index,cable_id);
        
    }
    else {
        cout << "Error" << endl;
    }
}

void CarpPurkinje::eraseRelation (unordered_multimap< std::vector<double>, unsigned int, MyHash>& hash, vector<double> key, unsigned int value){
    bool is_found = false;
    
    auto search = hash.equal_range(key);
    auto it = search.first;
    while (!is_found && search.second != it){
        if (it->second == value){
            hash.erase(it);
            is_found = true;
        }
        else
            ++it;
    }
    
}

void CarpPurkinje::addRelations (unsigned int origin_id, unsigned int new_cable_id, unsigned int end_id) {
    
    vector<double> new_point_beg, new_point_end;
    vector<double> new_cable;
    
    elements->getData(new_cable_id, new_cable);
    points->getData(new_cable[0], new_point_beg);
    points->getData(new_cable[1], new_point_end);
    
    searchParents.insert(pair< vector<double>, unsigned int>(new_point_beg, origin_id));
    searchParents.insert(pair< vector<double>, unsigned int>(new_point_end, new_cable_id));
    
    searchSons.insert(pair< vector<double>, unsigned int>(new_point_beg, new_cable_id));
    searchSons.insert(pair< vector<double>, unsigned int>(new_point_beg, end_id));
    
}

size_t CarpPurkinje::createNewElement(unsigned int id_attach_end, unsigned int id_attach_beg, unsigned int attach_point_id){
    vector<double> cable_attach_end, cable_attach_beg, new_cable;
    vector<double> point_end, point_beg;
    
    elements->getData(id_attach_end, cable_attach_end);
    size_t last_index = cable_attach_end.size() - 1;
    
    elements->getData(id_attach_beg, cable_attach_beg);
    
    new_cable.push_back(attach_point_id);
    new_cable.push_back(cable_attach_beg[0]);
    
    points->getData(cable_attach_end[last_index], point_end);
    eraseRelation(searchParents, point_end, id_attach_end);
    cable_attach_end[last_index] = attach_point_id;
    
    points->getData(cable_attach_beg[0], point_beg);
    eraseRelation(searchSons, point_beg, id_attach_beg);
    cable_attach_beg[0] = attach_point_id;
    
    elements->modifyData(id_attach_end, cable_attach_end);
    elements->modifyData(id_attach_beg, cable_attach_beg);
    elements->addData(new_cable);

    
    return (elements->size() - 1);
}

/**
 * Función que asigna como hijo una fibra de Purkinje a otra.
 * 
 * @param [in]  index   Índice de la fibra a la que se le asignará la fibra hijo.
 * @param [in]  id_hijo Índice de la fibra hijo.
 *
void CarpPurkinje::setSons(unsigned int index, unsigned int id_hijo) {
    
        if (purkinje_fiber[index].sons[0] == -1) {
            purkinje_fiber[index].sons[0] = id_hijo;
        }
        else if (purkinje_fiber[index].sons[1] == -1){
            purkinje_fiber[index].sons[1] = id_hijo;
        }
        else {
            cout << "Warning: el cable " << index << " tiene mas de dos hijos" << endl;
            changePurkinje(index, id_hijo);
        }
}*/


/**
 * Función que asigna como padre una fibra de Purkinje a otra.
 * 
 * @param [in]  index   Índice de la fibra a la que se le asignará la fibra padre.
 * @param [in]  id_padre Índice de la fibra padre.
 **
void CarpPurkinje::setFathers(unsigned int index, unsigned int id_padre) {
    
    if (purkinje_fiber[index].parents[0] == -1) {
        purkinje_fiber[index].parents[0] = id_padre;
    }
    else if (purkinje_fiber[index].parents[1] == -1) {
        purkinje_fiber[index].parents[1] = id_padre;
    }
    else {
        cout << "Error: el cable " << index << " tiene mas de dos padres." << endl;
    }
}
*/
size_t CarpPurkinje::createNewPoint (vector<double>& origin, vector<double>& end, float magnitud_vector) {
    
    vector<double> vector, new_point_coords;
    
    transform(end.begin(), end.end(), origin.begin(), back_inserter(vector), minus<double>());
    
    for (int i = 0; i < 3; ++i){
        vector[i] *= magnitud_vector;
    }
    transform(origin.begin(), origin.end(), vector.begin(), back_inserter(new_point_coords), plus<double>());
    
    points->addData(new_point_coords);
    
    return (points->size()-1);
}

/**
 * Función que escribe los datos necesarios y con la sintaxis adecuada a
 * cualquier tipo de "output stream". En el caso de no exisir ningún punto no 
 * crea el fichero.
 * 
 * @param [in,out]  where   "Output stream" en el que se escribirá la info.
 **/
void CarpPurkinje::print(std::ostream& where) const{
    
    unsigned int num_cables = elements->size();
    unsigned int nodes_amount;
    vector<double> nodes;
    vector<double> node_coords;
    
    where << num_cables << "\n";
    where << "########################################" << "\n";
    
    cout << num_cables << endl;
    for (unsigned int i = 0; i < num_cables; ++i){
        
        elements->getData(i, nodes);
        nodes_amount = nodes.size();
        
        where << "Cable " << i << "\n";
        /*where << fiber.parents[0] << " " << fiber.parents[1] << "\n";
        where << fiber.sons[0] << " " << fiber.sons[1] << "\n";*/
        printRelations(where, nodes);
        where << nodes_amount << "\n";
        where << cable_size << "\n";
        where << gap_resistance << "\n";
        where << conductivity << "\n";
        
        for (unsigned int j = 0; j < nodes_amount; ++j){
            points->getData(nodes[j], node_coords);
            for (auto a : node_coords)
                where << a << " ";
            where << "\n";
        }
        
        where << "########################################" << "\n";
        
    }
    
    where << "\0";
}

void CarpPurkinje::printRelations(std::ostream& where, vector<double> cable) const{
    
    vector<double> coords_beg, coords_end;
    size_t last_index = cable.size() - 1;
    
    PurkinjeRelations pr;
    
    points->getData(cable[0], coords_beg);
    points->getData(cable[last_index], coords_end);
    
    auto range = searchParents.equal_range(coords_beg);
    int i = 0;
    for (auto it = range.first; it != range.second; ++it){
        pr.parents[i] = it->second;
        ++i;
    }
    
    range = searchSons.equal_range(coords_end);
    i = 0;
    for (auto it = range.first; it != range.second; ++it){
        pr.sons[i] = it->second;
        ++i;
    }
    
    if (pr.parents[1] == -1){
        where << pr.parents[0] << " " << pr.parents[1];
    }
    else if (pr.parents[0] < pr.parents[1]){
        where << pr.parents[0] << " " << pr.parents[1];
    }
    else
        where << pr.parents[1] << " " << pr.parents[0];
    where << "\n";
    
    if (pr.sons[1] == -1){
        where << pr.sons[0] << " " << pr.sons[1];
    }
    else if (pr.sons[0] < pr.sons[1]){
        where << pr.sons[0] << " " << pr.sons[1];
    }
    else
        where << pr.sons[1] << " " << pr.sons[0];
    where << "\n";
}


/*
 * 
 * size_t CarpPurkinje::createNewCables (unsigned int index_new_point, unsigned int cable_to_divide_index, unsigned int index_cable_attach){  
    
    vector<double> cable_to_divide;
    elements->getData(cable_to_divide_index, cable_to_divide);
    size_t last_index = cable_to_divide.size() - 1;
    
    vector<double> new_cable;
    new_cable.push_back(index_new_point);
    new_cable.push_back(cable_to_divide[last_index]);
    elements->addData(new_cable);
    
    cable_to_divide[last_index] = index_new_point;
    elements->modifyData(cable_to_divide_index, cable_to_divide);
    
    vector<double> new_cable_son;
    elements->getData(index_cable_attach, new_cable_son);
    new_cable_son[0] = index_new_point;
    elements->modifyData(index_cable_attach, new_cable_son);
    
    return (elements->size()-1);
}
 * void CarpPurkinje::changePurkinje(unsigned int cable_index, unsigned int index_cable_attach) {
    size_t new_point_index = createNewPoint(cable_index);
    size_t new_cable_index = createNewCables (new_point_index, cable_index, index_cable_attach);
    changeSearchRelations (cable_index , new_cable_index);
    changePurkinjeRaltions(cable_index, new_cable_index, index_cable_attach);

    vector<double> new_cable;
    elements->getData(new_cable_index, new_cable);
    unsigned int last_index = new_cable.size() - 1;
    
    vector<double> coords_beg, coords_end;
    points->getData(new_cable[0], coords_beg);
    points->getData(new_cable[last_index], coords_end);
    
    searchParents.insert(pair< vector<double>, unsigned int>(coords_end, new_cable_index));
    searchSons.insert(pair< vector<double>, unsigned int>( coords_beg, new_cable_index));
    
}
 void CarpPurkinje::divideCableeee(unsigned int cable_to_divide_index, unsigned int id_hijo) {
    
    vector<double> cable_to_divide;
    vector<double> origin_coords, end_coords;
    vector<double> vector_coords;
    vector<double> new_point_coords;
    
    ///////////////////////////
    elements->getData(cable_to_divide_index, cable_to_divide);
    size_t last_index = cable_to_divide.size() - 1;
    size_t penultimate_index = last_index -1;
    
    points->getData(cable_to_divide[penultimate_index], origin_coords);
    points->getData(cable_to_divide[last_index], end_coords);
    std::trasform(end_coords.begin(), end_coords.end(), origin_coords.begin(), vector_coords.begin(), std::minus<double>());
    
    for (int i = 0; i < 3; ++i){
        vector_coords[i] *= 0.9;//
    }
    std::transform(origin_coords.begin(), origin_coords.end(), vector_coords.begin(), new_point_coords.begin(), std::plus<double>());
    
    points->addData(new_point_coords);
    size_t index_new_point = points->size()-1;
    ////////////////////////////////
    
    ////////////////////////////////////
    vector<double> new_cable;
    new_cable.push_back(index_new_point);
    new_cable.push_back(cable_to_divide[last_index]);
    elements->addData(new_cable);
    size_t index_new_element = elements->size()-1;
    ////////////////////////////////////
    
    ////////////////////////////////////
    cable_to_divide[last_index] = index_new_point;
    elements->modifyData(cable_index, cable_to_divide);
    /////////////////////////////////////
    
    /////////////////////////////////////
    auto search = searchParents.equal_range(end_coords);
    bool is_found = false;
    auto it = search.first;
    while (!is_found && search.second != it) {
        if (it->second == cable_index){
            searchParents.erase(it);
            is_found = true;
        }
        else 
            ++it;
    }
    /////////////////////////////////////////////////
    
    ///////////////////////////////
    PurkinjeRelations aux;
    aux.parents[0] = cable_index;
    aux.sons[0] = purkinje_fiber[cable_index].sons[0];
    aux.sons[1] = purkinje_fiber[cable_index].sons[1];
    purkinje_fiber.push_back(aux);
    purkinje_fiber[cable_index].sons[0] = id_hijo;
    purkinje_fiber[cable_index].sons[1] = index_new_element;
    ///////////////////////////////
    
    ///////////////////////////////////////////////
    vector<double> new_cable_son;
    elements->getData(id_hijo, new_cable_son);
    new_cable_son[0] = index_new_point;
    elements->modifyData(id_hijo, new_cable_son);
    ////////////////////////////////////////////////////
}
 * 
 * 
 * void CarpPurkinje::changePurkinjeRaltions (unsigned int cable_to_divide_index, unsigned int new_cable_index, unsigned int index_cable_attach) {
    PurkinjeRelations aux;
    aux.parents[0] = cable_to_divide_index;
    aux.sons[0] = purkinje_fiber[cable_to_divide_index].sons[0];
    aux.sons[1] = purkinje_fiber[cable_to_divide_index].sons[1];
    purkinje_fiber.push_back(aux);
    purkinje_fiber[cable_to_divide_index].sons[0] = index_cable_attach;
    purkinje_fiber[cable_to_divide_index].sons[1] = new_cable_index;
}
 * 
 * 
 * void CarpPurkinje::changeSearchRelations (unsigned int cable_to_divide_index, unsigned int new_cable_index) {
    
    vector<double> cable_to_divide;
    elements->getData(cable_to_divide_index, cable_to_divide);
    size_t last_index = cable_to_divide.size() - 1;
    
    vector<double> end_coords;
    points->getData(cable_to_divide[last_index], end_coords);
    
    auto search = searchParents.equal_range(end_coords);
    bool is_found = false;
    auto it = search.first;
    while (!is_found && search.second != it) {
        if (it->second == cable_to_divide_index){
            searchParents.erase(it);
            is_found = true;
        }
        else 
            ++it;
    }
    
    searchParents.insert(pair< vector<double>, unsigned int>(end_coords, new_cable_index));
    
    void CarpPurkinje::calcularVecinos2() {
    
    vector<double> fiber;
    vector<double> coords_beg, coords_end;
    size_t last_index;
    
    //Carefull
    size_t number_of_elements = elements->size();
    cout << number_of_elements << endl;
    for (size_t i = 0; i < number_of_elements; ++i) {
        elements->getData(i, fiber);
        last_index = elements->getDataDimension(i) - 1;
        
        points->getData(fiber[0], coords_beg);
        points->getData(fiber[last_index], coords_end);

        
        auto search =  searchParents.equal_range(coords_beg);
        for (auto it = search.first; it != search.second; ++it){
            setFathers(i, it->second); //Se ha encontrado un padre de esta fibra
            setSons (it->second, i);  //Y los hijos
        }
        
        search = searchSons.equal_range(coords_end);
        for (auto it = search.first; it != search.second; ++it){
            setSons(i, it->second); //Se ha encontrado un hijo de esta fibra
            setFathers (it->second, i);
        }
        
        searchParents.insert(pair< vector<double>, unsigned int>(coords_end, i));
        searchSons.insert(pair< vector<double>, unsigned int>( coords_beg, i));
        
    }
}
}
 */