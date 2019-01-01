/**
 * @file AbstractFile.h
 * 
 * Clase abstracta que representa a un fichero.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#ifndef OUTPUTFILES_H
#define OUTPUTFILES_H

#include <string>
#include <iostream>

class AbstractFile {
public:
    
    /**
     * Constructor. Inicializa el nombre y la extensión del fichero.
     * 
     * @param [in]  name        Nombre del fichero.
     * @param [in]  extension   Extension del fichero (punto incluido)
     **/
    AbstractFile(const std::string& name, const std::string& extension) {
        this->name = name;
        this->extension = extension;
    }
    
    /**
     * Función que devuelve el nombre del fichero.
     * 
     * @return String con el nombre.
     **/
    std::string getName () {
        return name;
    }
    
    /**
     * Función que devuelve la extensión del fichero.
     * 
     * @return String con la extensión.
     **/
    std::string getExtension() {
        return extension;
    }
    
    /**
     * Función que escribe los datos necesarios y con la sintaxis adecuada a
     * cualquier tipo de "output stream".
     * 
     * @param [in,out]  where   "Output stream en el que se escribirá la info.
     **/
    virtual void print(std::ostream& where) const = 0;
    
    
    /**
     * Función amiga que sobrecarga del operador de inserción para facilitar la
     * escritura a ficheros físicos.
     * 
     * @param [in,out]  out     "Output stream" en el que se escribirá el fichero.
     * @param [in]      file    El fichero a escribir.
     * @return  El "output stream" con la información escrita.
     **/
    friend std::ostream& operator<<(std::ostream& out, const AbstractFile& file) {
        file.print(out);
        return out;
    };
    
    virtual ~AbstractFile() {};
    
private:
    std::string name; ///< Nombre del fichero.
    std::string extension; ///< Extensión del fichero.

};

#endif /* OUTPUTFILES_H */

