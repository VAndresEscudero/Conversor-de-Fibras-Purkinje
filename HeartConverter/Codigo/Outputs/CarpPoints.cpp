/**
 * @file CarpPoints.cpp
 * 
 * Clase que representa un fichero de puntos (.pts) en CARP.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#include "CarpPoints.h"
#include <string>
#include <iostream>
#include "./../Datasets/DatasetAbstract.h"
using namespace std;

/**
 * Constructor. Inicializa el nombre del fichero. La extensión se asigna de
 * forma automática (.pts).
 * 
 * @param [in]  name    Nombre del fichero
 **/
CarpPoints::CarpPoints(const string& name) : AbstractFile(name, string(".pts")) {
    points = DatasetAbstract::getDataset("points");
}

/**
 * Función que escribe los datos necesarios y con la sintaxis adecuada a
 * cualquier tipo de "output stream". En el caso de no exisir ningún punto no 
 * crea el fichero.
 * 
 * @param [in,out]  where   "Output stream" en el que se escribirá la info.
 **/
void CarpPoints::print(std::ostream& where) const {

    size_t points_size = points->size();
    
    if (points_size == 0){
        cout << "Error: No existe ningun punto" << endl;
        cout << "No se puede crear un fichero de puntos" << endl;
        return;
    }
    
    where << points_size << "\n";
    
    vector<double> coords;
    for (size_t i = 0; i < points_size; ++i) {
        size_t points_dimension = points->getDataDimension(i);
        points->getData(i, coords);
        
        where << coords[0];
        for (size_t j = 1; j < points_dimension; ++j){
            where << " " << coords[j];
        }
        where << "\n";
    }
    
    where << "\0";
}

