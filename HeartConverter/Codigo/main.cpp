/**
 * @file main.cpp
 * 
 * Conversor de ficheros vtk a CARP. Este programa recibe un fichero
 * suministrado por el usuario y genera otros ficheros compatibles con CARP.
 * 
 * @author  Víctor Guillermo Andrés Escudero
 * @date    01/08/2018
 * @version 1.0
 * 
 **/

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "VtkParser.h"
#include "Outputs/AbstractFile.h"
#include "Outputs/CarpPoints.h"
#include "Outputs//CarpPurkinje.h"
#include "Outputs/CarpElements.h"
using namespace std;

/**
 * Structura que engloba la información necesaria para ejecutar el programa.
 **/
struct Parameters{
    string input_file;  ///< Ruta del archivo de entrada que sera convertido.
    string output_file; ///< Ruta del archivo de salida que creará el programa.
    string mode;        ///< El tipo de archivos que se obtendran.
};

Parameters printHelpMessage();
Parameters parseParameters(int, char**);
void sanitizeParameters (Parameters&);

string charArrayToLower(const string&);
char* charArrayToLower(char*);

void runProgram(Parameters);

/**
 * Programa principal.
 * Comprueba si el programa ha sido lanzado con argumentos, de no ser así
 * muestra al usuario un mensage de ayuda para que se los suministre.
 * Despues ejecuta el programa con los  parámetros suministrados.
 * 
 * @param [in]  argc    Número de arg. suministrados por linea de comandos.
 * @param [in]  argv    Vector de arg. suministrados por la linea de comandos.
 * @return              Devuelve exit_succes si ha sido un exito.
 **/
int main(int argc, char* argv[]) {
    Parameters parameters;
    
    
    if (argc < 2){
        parameters = printHelpMessage();
    }
    else {
        parameters = parseParameters(argc, argv);
    }
    
    sanitizeParameters(parameters);
    
    runProgram(parameters);
    
    
    return EXIT_SUCCESS;    
}

/**
 * Inicializa las clases necesarias para ejecutar el programa. Y comprueba
 * que los parámetros suministrados sean válidos.
 * 
 * @param [in]  p Structura que contiene la información necesaria para ejecutar el programa.
 **/
void runProgram(Parameters p) {
    vector<AbstractFile*> ficheros;
    
    if (p.mode == "h" || p.mode == "heart" ||
        p.mode == "p" || p.mode == "purkinje") {
        
        VtkParser parser(p.input_file.c_str());
        parser.createDatasets();
        
        if (p.mode == "h" || p.mode == "heart"){
            ficheros.push_back(new CarpElements(p.output_file));
            ficheros.push_back(new CarpPoints(p.output_file));
        }
        else if (p.mode == "p" || p.mode == "purkinje"){
            ficheros.push_back(new CarpPurkinje(p.output_file));
        }
    }
    else {
                
        cout << "This type of output file is not recognized." << endl;
        cout << "Try HEART or H for .pts and .elem files." << endl;
        cout << "Try PURKINJE or P for .pkje file" << endl;
    }
    
    //Run the program
    for (unsigned int i = 0; i < ficheros.size(); ++i){
        string file_name;
        ofstream file;
        
        file_name = ficheros[i]->getName() + ficheros[i]->getExtension();
        
        file.open(file_name);
        file << *ficheros[i];
        file.close();
    }
    
}

/**
 * Pide por consola los parámetros necesarios para ejecutar el programa.
 * 
 * @return Una instancia de la estructura Parameters con la información dada.
 **/
Parameters printHelpMessage() {
    Parameters parameters;
    
    cout << "No parameters found." << endl;
    cout << "Enter the parameters below." << endl << endl;
    
    cout << "Input file path: " << endl;
    getline(cin, parameters.input_file);
    cout << endl;
    
    cout << "Output file path: (same as input if left blank)" << endl;
    getline(cin, parameters.output_file);
    cout << endl;
    
    string mode;
    cout << "Convert the file into CARP's HEART or PURKINJE files? (HEART/PURKINJE)" << endl;
    //cin >> mode;
    getline(cin, parameters.mode);
    cout << endl;
    
    return parameters;
    
}

/**
 * Parsea los parámetros suministrados por linea de comandos. El programa busca
 * las siguientes "flags" -o (-output), -i (-input), -m (-mode) y toma el
 * siguiente parámetro como el valor suministrado por el usuario.
 * 
 * @param [in]  argc    Número de arg. suministrados por linea de comandos.
 * @param [in]  argv    Vector de arg. suministrados por la linea de comandos.
 * @return Una instancia de la estructura Parameters con la información dada.
 **/
Parameters parseParameters(int argc, char* argv[]) {
    char* p;
    Parameters parameters;
    //-o -i -m 
    for (int i = 1; i < (argc-1); ++i){
        p = charArrayToLower(argv[i]);
        
        if (strcmp(p, "-o") == 0 || strcmp(p, "-output") == 0){
            parameters.output_file = argv[i+1];
            ++i;
        }
        else if (strcmp(p, "-i") == 0 || strcmp(p, "-input") == 0) {
            parameters.input_file =  argv[i+1];
            ++i;
        }
        else if (strcmp(p, "-m") == 0 || strcmp(p, "-mode") == 0) {
            parameters.mode = argv[i+1];
            ++i;
        }
        else {
            cout << "Parameter " << p << " wasn't recognized. Try again." << endl;
        }
    }
    
    return parameters;
}

/**
 * Modifica los parámetros para que puedan ser parseados más facilmente en un
 * futuro. Si el fichero de salida esta vacio se le asigna la misma ruta y
 * nombre que el de entrada. Se elimina la extensión del fichero de salida y se
 * pasa el modo a minusculas.
 * 
 * @param [in,out]  p   Structura que contiene la información necesaria para ejecutar el programa.
 **/
void sanitizeParameters(Parameters& p) {
    if (p.output_file == ""){
        p.output_file = p.input_file;
    }
    
    p.output_file = p.output_file.substr(0, p.output_file.find(".", 0));
    p.mode = charArrayToLower(p.mode);
}

/**
 * Crea un string identico al original con las letras en minúsculas.
 * 
 * @param [in]  input   String de entrada.
 * @return Un string sin ninguna letra mayúscula.
 */
string charArrayToLower (const string& input) {
    string output = "";
    for (auto a : input){
        output += tolower(a);
    }
    return output;
}

/**
 * Crea un vector de carácteres identico al original con letras minúsculas.
 * 
 * @param [in]  input   Vector de carácteres de entrada.
 * @return Un string sin ninguna letra mayúscula.
 */
char* charArrayToLower(char* input) {
    int i = 0;
    char* output;
    
    while (input[i]){
        input[i] = tolower(input[i]);
        ++i;
    }
    
    return input;
}
