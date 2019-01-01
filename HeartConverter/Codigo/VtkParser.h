/**
 * @file VtkParser.h
 * 
 * Clase que lee un fichero de VTK y extrae información sobre sus puntos,
 * elementos, e información adicional.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#ifndef VTKPARSER_H
#define VTKPARSER_H

#include <string>
#include <vtkSmartPointer.h>
#include <vtkDataSet.h>
#include <vtkDataArray.h>
#include <memory>

class VtkParser {
public:
    vtkSmartPointer<vtkDataSet> vtk_data; ///< Puntero al conjunto de datos.
    
    VtkParser(const char*);
    
    void createDatasets();
    
private:    
    void createPoints();
    void createElements();
    void createAttributes(int);
    void createAttributeFromArray (vtkSmartPointer<vtkDataArray>);
    
    static std::string vtkTypeToNative (int);
};

#endif /* VTKPARSER_H */

