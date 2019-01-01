/**
 * @file CarpPoints.h
 * 
 * Clase que representa un fichero de puntos (.pts) en CARP.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#ifndef CARPPOINTS_H
#define CARPPOINTS_H

#include "AbstractFile.h"
#include <string>
#include <iostream>

class DatasetAbstract;

class CarpPoints : public AbstractFile{
public:

    CarpPoints(const std::string&);
    
    void print(std::ostream&) const;
    
private:
    DatasetAbstract* points; ///< Puntero a las coordenadas de todos los puntos.

};

#endif /* CARPPOINTS_H */

