/**
 * @file CarpElements.h
 * 
 * Clase que representa un fichero de elementos (.elem) en Carp.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#ifndef CARPELEMENTS_H
#define CARPELEMENTS_H

#include "AbstractFile.h"
#include <string>
#include <vector>

class DatasetAbstract;

class CarpElements : public AbstractFile{
public:
    CarpElements(const std::string&);
    
    void print(std::ostream&) const;
private:
    DatasetAbstract* points;                ///< Puntero a las coordenadas de los puntos.
    DatasetAbstract* elements;              ///< Puntero a los índices de los puntos que componen cada elemento.
    DatasetAbstract* primitives;            ///< Puntero al tipo de primitiva de cada elemento.
    DatasetAbstract* regions;               ///< Puntero a la región asignada para cada elemento.
    std::vector<std::string> primitives_tag;///< Vector con el "tag" apropiado de CARP que identifica la primitiva de cada elemento.
    
    void calcPrimitives();
    std::string getPrimitiveTag(int);

};

#endif /* CARPELEMENTS_H */

