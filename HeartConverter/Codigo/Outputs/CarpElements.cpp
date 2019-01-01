/**
 * @file CarpElements.cpp
 * 
 * Clase que representa un fichero de elementos (.elem) en Carp.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#include "CarpElements.h"
#include "./../Datasets/DatasetAbstract.h"
#include "vtkCellType.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

/**
 * Constructor. Llama al constructor de la clase de la que hereda para
 * inicializar los valores del nombre y extensión del fichero (.elem). También
 * inicializa los punteros a la información necesaria. También convierte la lista
 * de enteros que simbolizan cada una de las primitivas en VTK a el string
 * equivalente en CARP.
 * 
 * @param [in]  name    Nombre del fichero.
 **/
CarpElements::CarpElements(const string& name) : AbstractFile(name, ".elem") {
    points = DatasetAbstract::getDataset("points");
    elements = DatasetAbstract::getDataset("elements");
    primitives = DatasetAbstract::getDataset("primitives");
    regions = DatasetAbstract::getDataset("regions");
    
    if (primitives != nullptr){
        calcPrimitives();
    }
}

/**
 * Función que compureba que el fichero de configuración exista o lo crea en 
 * caso contrario. Tras esto asigna los valores del fichero a los parámetros de
 * la clase.
 **/
void CarpElements::print(std::ostream& where) const{
    
    vector<double> element;
    vector<double> region;
    
    where << elements->size() << "\n";
    
    for (unsigned int i = 0; i < elements->size(); ++i) {
        elements->getData(i, element);
        
        where << primitives_tag[i];
        
        for (auto index : element){
            where << " " << index;
        }
        
        if (regions != nullptr){
            regions->getData(i, region);
            where << " " <<region[0];
        }
        
        where << endl;
    }
    
}

/**
 * Itera sobre cada una de las primitivas de cada elemento obteniendo y 
 * almacenando su string equivalente en CARP.
 **/
void CarpElements::calcPrimitives() {
    string tag;
    vector<double> primitive;
    
    for (unsigned int i = 0; i < primitives->size(); ++i){
        primitives->getData(i, primitive);
        tag = getPrimitiveTag(primitive[0]);
        primitives_tag.push_back(tag);
    }
}

/**
 * Devuelve el string equivalente en CARP para los distintos tipos de primitiva
 * de VTK. Si el tipo de primitiva no esta implementado en CARP se lanza un
 * mensaje de error y se devuelve un string vacio. La traducción entero-string 
 * esta basado y debe de mantenerse actualizado de acuerdo al fichero
 * vtkCellType.h.
 * 
 * @param [in]  type    Entero que representa el tipo de primitiva en VTK.
 * @return String con el tag adecuado o un string vacio si CARP no soporta
 *         el tipo de primitiva.
 **/
string CarpElements::getPrimitiveTag(const int type) {
    //vtkCellType cell_type;
    
    if (type == VTKCellType::VTK_LINE)
        return "Ln";
    else if (type == VTKCellType::VTK_TRIANGLE)
        return "Tr";
    else if (type == VTKCellType::VTK_POLYGON)
        return "Tr";
    else if (type == VTKCellType::VTK_QUAD)
        return "Qd";
    else if (type == VTKCellType::VTK_TETRA)
        return "Tt";
    else if (type == VTKCellType::VTK_PYRAMID)
        return "Py";
    else if (type == VTKCellType::VTK_WEDGE)
        return "Pr";
    else if (type == VTKCellType::VTK_HEXAHEDRON)
        return "Hx";
    else{
        cout << "Primitive not supported." << endl;
        return "";
    }
}





