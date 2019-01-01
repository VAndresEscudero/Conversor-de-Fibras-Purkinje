/**
 * @file VtkParser.cpp
 * 
 * Clase que lee un fichero de VTK y extrae información sobre sus puntos,
 * elementos, e información adicional.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#include "vtkSmartPointer.h"
#include "vtkDataSetReader.h"
#include "vtkIdList.h"
#include <vtkFieldData.h>
#include <vtkCell.h>
#include <vtkType.h>
#include <vtkCellType.h>

#include "Datasets/DatasetAbstract.h"
#include "VtkParser.h"

#include "vector"
#include "string"
#include <iostream>
using namespace std;

/**
 * Constructor. Lee el fichero de entrada y crea un objeto con toda la
 * información disponible.
 * 
 * @param [in]  file_name   Nombre del fichero de entrada.
 **/
VtkParser::VtkParser(const char* file_name) {
    
    auto reader = vtkSmartPointer<vtkDataSetReader>::New();
    reader->SetFileName(file_name);
    reader->Update();
    
    vtk_data = reader->GetOutput();
}


/**
 * Función de ayuda. Llama a otras funciones para obtener los datos del
 * conjunto de datos.
 **/
void VtkParser::createDatasets() {

    //GetPoints
    createPoints();
    
    //GetElements
    createElements();
    
    //GetArrays
    createAttributes(vtkDataSet::AttributeTypes::POINT);
    createAttributes(vtkDataSet::AttributeTypes::CELL);
    createAttributes(vtkDataSet::AttributeTypes::FIELD);
}


/**
 * Obtiene las coordenadas de los puntos que se encuentran en el fichero y los
 * almacena internamente para poder ser usados más adelante.
 **/
void VtkParser::createPoints() {
    
    unsigned int size = vtk_data->GetNumberOfPoints();
    
    DatasetAbstract* points = DatasetAbstract::FactoryDataset("double", "points", size);
    
    vtkIdType id[1];
    double coords[3];
    
    for (unsigned int i = 0; i < size; ++i){
        
        id[0] = i;
        vtk_data->GetPoint(id[0], coords);
        points->addData(coords, 3);
    }
    
    
}

/**
 * Obtiene para cada elemento que tipo es (linea/triangulo/prisma etc) y los
 * indices de los puntos que los componen.
 **/
void VtkParser::createElements() {
    
    unsigned int size = vtk_data->GetNumberOfCells();
    
    DatasetAbstract* elements = DatasetAbstract::FactoryDataset("unsigned int", "elements", size);
    DatasetAbstract* primitives = DatasetAbstract::FactoryDataset("unsigned short", "primitives", size);
    
    vtkIdType point_id[1], cell_id[1];
    
    for (unsigned int i = 0; i < size; ++i) {
        cell_id[0] = i;
        auto cell = vtk_data->GetCell(cell_id[0]);
        
        double primitive[1];
        primitive[0] = cell->GetCellType();
        primitives->addData(primitive, 1);
        
        auto index_list = cell->GetPointIds();
        vector<double> element_ids;
        
        for (unsigned int j = 0; j < index_list->GetNumberOfIds(); ++j) {
            point_id[0] = j; 
            element_ids.push_back(index_list->GetId(point_id[0]));
        }
        
        elements->addData(element_ids);         
    }
    
}

/**
 * Obtiene los arrays que contienen los atributos del conjuto de datos
 * asociados con los puntos, elementos o del propio conj. de datos. También los
 * almacena (con el nombre que se le ha dado en el fichero).
 * 
 * @param [in]  type    Entero que identifica los atributos del tipo de dato que
 *                      se quiere obtener. De acuerdo al enum disponible en la
 *                      clase vtkDataObject de VTK.
 **/
void VtkParser::createAttributes(int type) {
    
    vtkSmartPointer<vtkFieldData> data_attribute = vtk_data->GetAttributesAsFieldData(type);
    
    for (unsigned int i = 0; i < data_attribute->GetNumberOfArrays(); ++i){
        vtkSmartPointer<vtkDataArray> data_array = data_attribute->GetArray(i);
        createAttributeFromArray (data_array);
    }
    
}

/**
 * Almacena un array con información sobre el conj. de datos. Extrae del array
 * su nombre y tipo de datos (int, float, etc) para almacenarlos de forma
 * eficiente.
 * 
 * @param [in]  array   Puntero a un array de vtk que se quiere almacenar.
 **/
void VtkParser::createAttributeFromArray(vtkSmartPointer<vtkDataArray> array){
    
    string array_name = array->GetName();
    string data_type = vtkTypeToNative(array->GetDataType());
    
    vtkIdType tuples_num = array->GetNumberOfTuples();
    int tuples_size = array->GetNumberOfComponents();
    
    DatasetAbstract* array_dataset = DatasetAbstract::FactoryDataset(data_type, array_name, tuples_num);
    
    for (vtkIdType tuple_id = 0; tuple_id < tuples_num; ++tuple_id) {
        double* tuples_data = array->GetTuple(tuple_id);
        
        vector<double> tuple_aux;
        for (int j = 0; j < tuples_size; ++j){
            tuple_aux.push_back(tuples_data[j]);
        }
        array_dataset->addData(tuple_aux);
    }
}


/**
 * Función que devuelve un string con el tipo de dato que representa el entero
 * de entrada. La traducción entero-string esta basado y debe de mantenerse
 * actualizado de acuerdo al fichero vtkType.h
 * 
 * @param [in]  data_type    Entero que representa un tipo de dato en VTK.
 * @return Un string con el tipo de dato equivalente nativo de c++.
 **/
string VtkParser::vtkTypeToNative(int data_type) {
    
    string type;
    
    switch (data_type) {
        case 0 : type = "void";
                 break;
        case 1 : type = "bool";
                 break;
        case 2 : type = "char";
                 break;
        case 3 : type = "unsigned char";
                 break;
        case 15: type = "signed char";
                 break;
        case 4 : type = "short";
                 break;
        case 5 : type = "unsigned short";
                 break;
        case 6 : type = "int";
                 break;
        case 7 : type = "unsigned int";
                 break;
        case 8 : type = "long";
                 break;
        case 9 : type = "unsigned long";
                 break;
        case 10: type = "float";
                 break;
        case 11: type = "double";
                 break;
        case 13: type = "string";
                 break;
        case 16: type = "long long";
                 break;
        case 17: type = "unsigned long long";
                 break;
        case 18: type = "int64_t";
                 break;
        case 19: type = "uint64_t";
                 break;
        default: type = "not suported";
                 break;
    }
    
    return type;
}

