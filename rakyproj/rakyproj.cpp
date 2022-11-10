// rakyproj.cpp : This file contains the 'main' function. Program execution begins and ends there.
//#include <cstdlib>
//#include <fstream>
//#include <stdio.h>
//#include <string.h>
//#include <iostream>
//#include "MeshProcessingViewer.h"
//#include <pmp/BoundingBox.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif
#include <cstdlib>
#include<iostream>
#include<fstream>
#include<stdio.h>
#include<string.h>
#include <pmp/SurfaceMesh.h>
#include <pmp/algorithms/Remeshing.h>
#include "MeshProcessingViewer.h"
#include "pmp/BoundingBox.h"
#include <string>
#include <vector>
#include "pmp/visualization/Window.h"
#include "pmp/algorithms/HoleFilling.h"
#include "pmp/MatVec.h"
#include <Eigen/Dense>
#include <pmp/algorithms/Triangulation.h>
#include "pmp/algorithms/DifferentialGeometry.h"
#include "Common_Func.h"
#include "shapedescriptor.h"
#include <iomanip>
#include "emd1.h"

using Eigen::MatrixXd;
using namespace pmp;
using namespace std;

void clearFile(string filePath)
{
    fstream fs;
    fs.open(filePath, ios::out); //这里写ios::trunc不能达到清空文件的效果
    fs.close();                  
}

Halfedge find_boundary(const SurfaceMesh& mesh)
{
    for (auto h : mesh.halfedges())
        if (mesh.is_boundary(h))
            return h;
    return Halfedge();
}

bool sortcol(const vector<double>& v1, const vector<double>& v2)
{
    return v1[1] < v2[1];
}

int main(int argc, char** argv)
{
    std::cout << "Readme: Please use the mesh in Labeled PSB folder only or the type classifying will not work properly\n";
    std::cout << "Notice: 261.off - 280.off is not avaliable\n";
    std::cout << "Do you want to skip examine before querying? (y/n)\n";
    string Type[20] = {"Human", "Cup",     "Glass",   "Airplane", "Ant",
                       "Chair", "Octopus", "Table",   "Teddy",    "Hand",
                       "Plier", "Fish",    "Bird",    " ",        "Armadillo",
                       "Bust",  "Mech",    "Bearing", "Vase",     "Fourleg"};
    //cup =2, glass = 2, bearing = 1
    int Typecnt[20] = {
        20, 18, 18, 20, 20, 20, 20, 20, 20, 20,
        20, 20, 20, 0,  20, 20, 20, 19, 20, 20,
    };
    
    char examinput, filepath[11] = "LabeledDB/", filetype[5] = ".off",
         csvpath[15] = "csv/report.csv";

    // Clear csv file and open
    fstream csvout;
    void clearFile(string(csvpath));
    csvout.open("csv/report.csv", ios::out);

    std::cin >> examinput;
    if (examinput == 'y')
    {
        
        /*std::cout << "Please enter the number of mesh: ";
        int meshinfo_size = 57;
        vector<vector<string>> content;
        vector<string> row;
        vector<double> selected_mesh;
        vector<double> Eucstore;
        vector<vector<double>> Euclideandistancearray;
        vector<double> Cosstore;
        vector<vector<double>> Cosdistancearray;
        string line, word, selected_meshclass;
        int meshno;
        double Euclideandistance;
        bool findno = false;
        std::cin >> meshno;
        selected_mesh.push_back(0);
        selected_mesh.push_back(0);*/

        //added
        int itera, iterend;
        char target[10][20];
        std::cout << "*************Querying started*************" << std::endl;
        std::cout << "where to start(min:1, max:400)?" << std::endl;
        cin >> itera;
        std::cout << "where to stop?(min:1, max:400)" << std::endl;
        std::cout << "If you selected 1 mesh, comparison window will be offered" << std::endl;
        cin >> iterend;
        bool awake_viewer = false;
        if (itera == iterend)
        {
            awake_viewer = true;
        }
        int numberofquery;
        ;
        std::cout << "Please enter K(number of query): " << std::endl;
        std::cin >> numberofquery;
        for (itera; itera <= iterend;
             itera++)
        {
            //added
            int meshinfo_size = 57;
            ifstream file("csv/no_outlier115.csv", ios::in);
            vector<vector<string>> content;
            vector<string> row;
            vector<double> selected_mesh;
            vector<double> Eucstore;
            vector<vector<double>> Euclideandistancearray;
            vector<double> Cosstore;
            vector<vector<double>> Cosdistancearray;
            std::vector<double> av, bv, aw, bw, a3v, d1v, d2v, d3v, d4v, a3w,
                d1w, d2w, d3w, d4w, ba3v, bd1v, bd2v, bd3v, bd4v, ba3w, bd1w,
                bd2w, bd3w, bd4w;
            vector<double> EMDstore;
            vector<vector<double>> EMDdistancearray;
            string line, word, selected_meshclass;
            
            double Euclideandistance;
            bool findno = false;
            selected_mesh.push_back(0);
            selected_mesh.push_back(0);

            //std::cout << "iter" << itera << "started" << std::endl;
            csvout << itera << ",";
            //int wordcnt = 0, rowcnt = 0;
            if (file.is_open())
            {

                while (getline(file, line))
                {
                    row.clear();
                    stringstream str(line);
                    while (getline(str, word, ','))
                    {
                        if (!findno && word == std::to_string(itera))
                        {
                            getline(str, word, ',');
                            selected_meshclass = word;
                            for (int i = 0; i < meshinfo_size; i++)
                            {
                                getline(str, word, ',');
                                selected_mesh.push_back(stod(word));
                            }
                            findno = true;
                        }
                        else
                        {
                            row.push_back(word);
                            //wordcnt++;
                        }
                    }
                    content.push_back(row);
                    //rowcnt++;
                    //cout << "iter " << rowcnt << " wordcnt is " << wordcnt << endl;
                }
            }
            else
                std::cout << "Could not open the file\n";
            
            std::cout << "You have selected " << itera << ".off, it is a " << selected_meshclass << std::endl;
            csvout << selected_meshclass << ",";

            /*for (int i = 0; i < content.size(); i++)
                cout << "content size, content[i] size: " << i
                     << ":" << content[i].size() << endl;*/
            /*for (int i = 0; i < content.size(); i++)
            {
                if (content[pointer].size() == 0)
                    continue;
                for (int j = 0; j < content[i].size(); j++)
                    std::cout << content[i][j] << ",";
            }*/
                
            //std::cout << "Mesh info: " << selected_mesh << std::endl;
            double Cossum = 0;

            //added
            for (int i = 0; i < content.size(); i++)
            {
                /*std::cout << "Pointer is " << i << " ";*/
                if (content[i].size() == 0)
                    continue;
                double Eucfactor = 0;
                double EMDEucfactor = 0;
                double Coschild = 0;
                double Cosparent = 1;
                double Cosfactor1 = 0;
                double Cosfactor2 = 0;
                Eucstore.clear();
                Cosstore.clear();
                EMDstore.clear();
                a3v.clear();
                a3w.clear();
                ba3v.clear();
                ba3w.clear();
                d1v.clear();
                d1w.clear();
                bd1v.clear();
                bd1w.clear();
                d2v.clear();
                d2w.clear();
                bd2v.clear();
                bd2w.clear();
                d3v.clear();
                d3w.clear();
                bd3v.clear();
                bd3w.clear();
                d4v.clear();
                d4w.clear();
                bd4v.clear();
                bd4w.clear();
                Eucstore.push_back(stod(content[i][0]));
                Cosstore.push_back(stod(content[i][0]));
                EMDstore.push_back(stod(content[i][0]));
                for (int j = 2; j < meshinfo_size; j++)
                {
                    //added
                    //EuclideanDistance
                    Eucfactor +=
                        pow((selected_mesh[j] - stod(content[i][j])), 2);
                    //CosDistance
                    Cosfactor1 = selected_mesh[j] * stod(content[i][j]);
                    Cosfactor2 = sqrt(pow(selected_mesh[j], 2) +
                                      pow(stod(content[i][j]), 2));
                    Coschild += Cosfactor1;
                    if (Cosfactor2 != 0)
                        Cosparent *= Cosfactor2;
                    //cout << content[i][j] << " ";
                    //EMD
                    if (j < 7)
                        EMDEucfactor += Eucfactor;
                    else if (j >= 7 && j < 17)
                    {
                        a3v.push_back(selected_mesh[j]);
                        ba3v.push_back(stod(content[i][j]));
                    }
                    else if (j >= 17 && j < 27)
                    {
                        d1v.push_back(selected_mesh[j]);
                        bd1v.push_back(stod(content[i][j]));
                    }
                    else if (j >= 27 && j < 37)
                    {
                        d2v.push_back(selected_mesh[j]);
                        bd2v.push_back(stod(content[i][j]));
                    }
                    else if (j >= 37 && j < 47)
                    {
                        d3v.push_back(selected_mesh[j]);
                        bd3v.push_back(stod(content[i][j]));
                    }
                    else
                    {
                        d4v.push_back(selected_mesh[j]);
                        bd4v.push_back(stod(content[i][j]));
                    }
                }
                //Euc
                Euclideandistance = sqrt(Eucfactor);
                //std::cout << "the Euclidean distance to " << content[i][0] << ".off is" << Euclideandistance << std::endl;
                Eucstore.push_back(Euclideandistance);
                Euclideandistancearray.push_back(Eucstore);
                
                //Cos
                Cosstore.push_back(abs(1 - (Coschild / Cosparent)));
                //std::cout << "Cosparent: " << Cosparent<< " Cosresult : " << 1 - (Coschild / Cosparent)<< std::endl;
                Cosdistancearray.push_back(Cosstore);
                
                //EMD here
                for (int j = 0; j < 10; j++)
                {
                    a3w.push_back(1);
                    ba3w.push_back(1);
                    d1w.push_back(1);
                    bd1w.push_back(1);
                    d2w.push_back(1);
                    bd2w.push_back(1);
                    d3w.push_back(1);
                    bd3w.push_back(1);
                    d4w.push_back(1);
                    bd4w.push_back(1);
                }

                //std::cout << "calculating EMD" << std::endl;
                //std::cout << "calculating EMD" << std::endl;
                EMDstore.push_back(log10(sqrt(EMDEucfactor)) +
                                   log10(wasserstein(a3v, a3w, ba3v, ba3w)) +
                                   log10(wasserstein(d1v, d1w, bd1v, bd1w)) +
                                   log10(wasserstein(d2v, d2w, bd2v, bd2w)) +
                                   log10(wasserstein(d3v, d3w, bd3v, bd3w)) +
                                   log10(wasserstein(d4v, d4w, bd4v, bd4w)));
                /*std::cout << "original 5: " << sqrt(EMDEucfactor) << ","
                          << wasserstein(a3v, a3w, ba3v, ba3w) << ","
                          << wasserstein(d1v, d1w, bd1v, bd1w) << ","
                          << wasserstein(d2v, d2w, bd2v, bd2w) << ","
                          << wasserstein(d3v, d3w, bd3v, bd3w) << ","
                          << wasserstein(d4v, d4w, bd4v, bd4w) << "\n";
                std::cout << "log10 : " << log10(sqrt(EMDEucfactor)) << ","
                          << log10(wasserstein(a3v, a3w, ba3v, ba3w)) << ","
                          << log10(wasserstein(d1v, d1w, bd1v, bd1w)) << ","
                          << log10(wasserstein(d2v, d2w, bd2v, bd2w)) << ","
                          << log10(wasserstein(d3v, d3w, bd3v, bd3w)) << ","
                          << log10(wasserstein(d4v, d4w, bd4v, bd4w)) << "\n";*/
                //std::cout << "the EMD to " << content[i][0] << ".off is"<< wasserstein(av, aw, bv, bw) << std::endl;
                EMDdistancearray.push_back(EMDstore);
            }

            //querying result
            
            //Euclideandistance querying
            
            std::sort(Euclideandistancearray.begin(), Euclideandistancearray.end(), sortcol);
            std::cout << "The most similar " << numberofquery 
                      << " shapes to this " << selected_meshclass
                      << " at Euclidean Distance are : " << std::endl;
            std::cout << "     Class         No. Distance " << std::endl;
            //d = content.size(), c = Typecnt[?/20], s = numberofquery
            double TPcnt = 0 ,FPcnt = 0, FNcnt = 0, TNcnt = 0;
            for (int i = 0; i < numberofquery; i++)
            {
                std::cout << setw(10)
                     << Type[(int(Euclideandistancearray[i][0]) - 1) / 20]
                     << " ";
                //added
                //csvout << Type[(int(Euclideandistancearray[i][0]) - 1) / 20]<< ",";
                
                for (int j = 0; j < Euclideandistancearray[i].size(); j++)
                {
                    std::cout << setw(10) << Euclideandistancearray[i][j];
                    //added
                    //csvout << Euclideandistancearray[i][j] << ",";
                }
                //TP
                if (selected_meshclass ==
                    Type[(int(Euclideandistancearray[i][0]) - 1) / 20])
                {
                    std::cout << " TP";
                    TPcnt++;
                }
                std::cout << endl;
            }
            //d = content.size(), c = Typecnt[(int(distancearray[i][0]) - 1) / 20], s = numberofquery
            FPcnt = numberofquery - TPcnt;
            //FN = c - TP
            FNcnt = Typecnt[(itera - 1) / 20] - TPcnt;
            //TN = d - c - FP
            TNcnt = content.size() - Typecnt[(itera - 1) / 20] -
                    FPcnt;
            if ((TPcnt + FPcnt + TNcnt + FNcnt) != content.size())
                std::cout << "error" ;
            double EucPre = TPcnt / (TPcnt + FPcnt);
            double EucNPV = TNcnt / (TNcnt + FNcnt);
            double EucSen = TPcnt / (TPcnt + FNcnt);
            double EucSpe = TNcnt / (TNcnt + FPcnt);
            double EucAccuracy = (TPcnt + TNcnt) / content.size();
            /*std::cout << TPcnt << ", " << FPcnt << ", " << FNcnt << ", " << TNcnt << ", "<< EucAccuracy<< std::endl;
            std::cout << EucPre << ", " << EucNPV << std::endl;
            std::cout << EucSen << ", " << EucSpe << std::endl;*/
            csvout << EucAccuracy << "," << EucPre << "," << EucNPV << ","
                   << EucSen << "," << EucSpe << ",";
                   
            //Cosine distance querying
            std::sort(Cosdistancearray.begin(), Cosdistancearray.end(),
                      sortcol);
            std::cout << "The most similar " << numberofquery
                      << " shapes to this " << selected_meshclass
                      << " at Cosine Distance are : " << std::endl;
            std::cout << "     Class              No.      Distance "
                      << std::endl;
            TPcnt = 0, FPcnt = 0, FNcnt = 0, TNcnt = 0;
            for (int i = Cosdistancearray.size() - 1;
                 i > Cosdistancearray.size() - numberofquery - 1; i--)
            {
                std::cout << setw(10)
                          << Type[(int(Cosdistancearray[i][0]) - 1) / 20]
                     << " ";
                //added
                //csvout << Type[(int(Cosdistancearray[i][0]) - 1) / 20] << ",";
                
                for (int j = 0; j < Cosdistancearray[i].size(); j++)
                {
                    std::cout << setw(15) << Cosdistancearray[i][j];
                    //added
                    //csvout << Cosdistancearray[i][j] << ",";
                }
                //TP
                if (selected_meshclass ==
                    Type[(int(Cosdistancearray[i][0]) - 1) / 20])
                {
                    std::cout << " TP";
                    TPcnt++;
                }
                std::cout << endl;
            }
            //d = content.size(), c = Typecnt[(int(distancearray[i][0]) - 1) / 20], s = numberofquery
            FPcnt = numberofquery - TPcnt;
            //FN = c - TP
            FNcnt = Typecnt[(itera - 1) / 20] - TPcnt;
            //TN = d - c - FP
            TNcnt = content.size() - Typecnt[(itera - 1) / 20] - FPcnt;
            if (TPcnt + FPcnt + TNcnt + FNcnt != content.size())
                std::cout << "error";
            double CosPre = TPcnt / (TPcnt + FPcnt);
            double CosNPV = TNcnt / (TNcnt + FNcnt);
            double CosSen = TPcnt / (TPcnt + FNcnt);
            double CosSpe = TNcnt / (TNcnt + FPcnt);
            double CosAccuracy = (TPcnt + TNcnt) / content.size();
            /*std::cout << TPcnt << ", " << CosAccuracy << std::endl;
            std::cout << CosPre << ", " << CosNPV << std::endl;
            std::cout << CosSen << ", " << CosSpe << std::endl;*/
            csvout << CosAccuracy << "," << CosPre << "," << CosNPV << ","
                   << CosSen << "," << CosSpe << ",";

            //Euclidean + EMD distance querying
            std::sort(EMDdistancearray.begin(),
                      EMDdistancearray.end(), sortcol);
            std::cout << "The most similar " << numberofquery
                      << " shapes to this " << selected_meshclass
                      << " at EMD Distance are : " << std::endl;
            std::cout << "     Class         No. Distance " << std::endl;
            TPcnt = 0, FPcnt = 0, FNcnt = 0, TNcnt = 0;
            for (int i = 0; i < numberofquery; i++)
            {
                std::cout << setw(10)
                          << Type[(int(EMDdistancearray[i][0]) - 1) / 20]
                          << " ";
                //added
                //csvout << Type[(int(EMDdistancearray[i][0]) - 1) / 20] << ",";
                
                for (int j = 0; j < EMDdistancearray[i].size(); j++)
                {
                    std::cout << setw(10) << EMDdistancearray[i][j];
                    //added
                    //csvout << EMDdistancearray[i][j] << ",";
                }
                //TP
                if (selected_meshclass ==
                    Type[(int(EMDdistancearray[i][0]) - 1) / 20])
                {
                    std::cout << " TP";
                    TPcnt++;
                }
                std::cout << endl;
            }
            //d = content.size(), c = Typecnt[(int(distancearray[i][0]) - 1) / 20], s = numberofquery
            FPcnt = numberofquery - TPcnt;
            //FN = c - TP
            FNcnt = Typecnt[(itera - 1) / 20] - TPcnt;
            //TN = d - c - FP
            TNcnt = content.size() - Typecnt[(itera - 1) / 20] - FPcnt;
            if (TPcnt + FPcnt + TNcnt + FNcnt != content.size())
                std::cout << "error";
            double EMDPre = TPcnt / (TPcnt + FPcnt);
            double EMDNPV = TNcnt / (TNcnt + FNcnt);
            double EMDSen = TPcnt / (TPcnt + FNcnt);
            double EMDSpe = TNcnt / (TNcnt + FPcnt);
            double EMDAccuracy = (TPcnt + TNcnt) / content.size();
            /*std::cout << TPcnt << ", " << EMDAccuracy << std::endl;
            std::cout << EMDPre << ", " << EMDNPV << std::endl;
            std::cout << EMDSen << ", " << EMDSpe << std::endl;*/
            csvout << EMDAccuracy << "," << EMDPre << "," << EMDNPV << ","
                   << EMDSen << "," << EMDSpe << ",";

            //when the number of mesh equals to 1, select preferred distance
            if (awake_viewer)
            {
                std::cout << "Which distance do you prefer? (e/c): "
                          << std::endl;
                std::cout << "e for Euclidean; c for Cosine, m for EMD"
                          << std::endl;
                std::cin >> examinput;
                if (examinput == 'e')
                {
                    for (int i = 0; i < numberofquery; i++)
                    {
                        std::sprintf(target[i], "%s%d%s", filepath,
                                     int(Euclideandistancearray[i][0]),
                                     filetype);
                    }
                }
                else if (examinput == 'c')
                {
                    int meshtotal = Cosdistancearray.size() - 1;
                    for (int i = meshtotal; i > meshtotal - numberofquery - 1; i--)
                    {
                        std::sprintf(target[meshtotal - i], "%s%d%s", filepath,
                                     int(Cosdistancearray[i][0]), filetype);
                        //std::cout << "target is " << target[meshtotal - i] << std::endl;
                    }
                }
                else if (examinput == 'm')
                {
                    //EMD here if avaliable
                    for (int i = 0; i < numberofquery; i++)
                    {
                        std::sprintf(target[i], "%s%d%s", filepath,
                                     int(EMDdistancearray[i][0]),
                                     filetype);
                    }     
                }
                else
                {
                    std::cout << "illegal input!" << std::endl;
                    return 0;
                }       
            }  

            //added
            content.clear();
            row.clear();
            selected_mesh.clear();
            Eucstore.clear();
            Cosstore.clear();
            EMDstore.clear();
            Euclideandistancearray.clear();
            Cosdistancearray.clear();
            EMDdistancearray.clear();
            csvout << std::endl;
            file.close();
            //std::cout << "iter" << itera << "ended" << std::endl;
            //csvout << "\n";
            if (itera == 24 || itera == 38 || itera == 57 || itera == 46 ||
                itera == 356)
                itera++;
            else if (itera == 260)
                itera = 280;
        }
        if (awake_viewer)
        {
            bool gui = true;
            MeshViewer viewer("MeshViewer: 1st", 800, 600, gui);
            viewer.load_mesh(target[0]);
            viewer.run();
            viewer.~MeshViewer();

            MeshViewer viewer2("MeshViewer: 2nd", 800, 600, gui);
            viewer2.load_mesh(target[1]);
            viewer2.run();
            viewer2.~MeshViewer();

            MeshViewer viewer3("MeshViewer: 3rd", 800, 600, gui);
            viewer3.load_mesh(target[2]);
            viewer3.run();
            viewer3.~MeshViewer();
            exit(1);
        }
        cin.get();
        return 0;
    }
    else
    {
        std::cout << "Enter y to scan all meshes for basic info / n for querying one mesh? (y/n)  ";
        char modeinput;
        int cycle;
        bool scan = true, autoscan = true;

        // tool mode select(single/all)
        std::cin >> modeinput;
        if (modeinput == 'y')
        {
            autoscan = true;
        }
        else
        {
            autoscan = false;
        }
        char filerealn[10];
        std::cout << "Please enter the mesh No.(min:1,max:400): ";
        std::cin >> cycle;
        int itera;
        while (scan)
        {
            // select scantype
            std::cout << "****************************************Raky Studio. "
                         "Inc***********************************************\n";

            // initialize
            char fileName[10];
            std::sprintf(fileName, "%s%d%s", filepath, cycle, filetype);
            std::sprintf(filerealn, "%s%d%s", filepath, cycle, filetype);
            std::puts(fileName);
            SurfaceMesh mesh;
            mesh.read(fileName);

            /************************Step 2********************************/
            //the class of the shape
            
            char* tok;
            tok = strtok(strtok(fileName, "."), "/");
            tok = strtok(NULL, "/");
            int filenum = atoi(tok);
            std::cout << "\nContent of " << cycle << filetype << ":\n";
            csvout << cycle << filetype << ",";
            std::cout << "Mesh theme: This is a " << Type[(cycle - 1) / 20]
                      << std::endl;
            csvout << Type[(cycle - 1) / 20] << ",";

            //
            size_t num_v = mesh.n_vertices();
            size_t num_f = mesh.n_faces();
            // get pre-defined property storing vertex positions
            auto points = mesh.get_vertex_property<Point>("v:point");
            MatrixXd A_o(num_v, 3); //Get the matrix A_o
            int i = 0;
            for (auto v : mesh.vertices())
            {
                A_o(i, 0) = points[v][0]; //x_coordinate
                A_o(i, 1) = points[v][1]; //y_coordinate
                A_o(i, 2) = points[v][2]; //z_coordinate
                i++;
            }
            Point p = A_o.colwise().mean(); //! [barycenter]
            std::cout << "barycenter: " << A_o.colwise().mean() << std::endl;

            //translation
            //MatrixXd A_trans = A_o.rowwise() - A_o.colwise().mean();

            //Computing posscalar
            MatrixXd becentered = A_o.rowwise() - A_o.colwise().mean();
            MatrixXd becov = becentered.adjoint() * becentered;
            Eigen::EigenSolver<MatrixXd> bees(becov);

            MatrixXd bee_v = bees.pseudoEigenvectors();
            Eigen::Vector3d bee1 = bee_v.row(0);

            /*double beforeposscalar = abs(bee1[0]) / abs(Distance(bee1, vec3(0, 0, 0)));
        std::cout << "before posscalar: " << beforeposscalar << std::endl;
        csvout << beforeposscalar << "; ";*/

            //Computing PCA in C++
            MatrixXd A_trans = A_o.rowwise() - A_o.colwise().mean();
            MatrixXd centered = A_trans.rowwise() - A_trans.colwise().mean();
            MatrixXd cov = centered.adjoint() * centered;
            Eigen::EigenSolver<MatrixXd> es(cov);

            MatrixXd e_v = es.pseudoEigenvectors();
            Eigen::Vector3d e1 = e_v.col(1);
            Eigen::Vector3d e2 = e_v.col(0);
            Eigen::Vector3d e3 = e1.cross(e2);

            //Alignment
            i = 0;
            for (auto v : mesh.vertices())
            {
                Eigen::Vector3d p_update = A_trans.row(i);
                points[v][0] = p_update.dot(e1);
                points[v][1] = p_update.dot(e2);
                points[v][2] = p_update.dot(e3);
                i++;
            }
            //mesh.write("align.off");

            //flipping
            double f0 = 0, f1 = 0, f2 = 0;
            for (auto f : mesh.faces()) // loop over all faces
            {
                Point c(0, 0, 0);
                Scalar n(0);
                for (auto v : mesh.vertices(f))
                {
                    c += mesh.position(v);
                    ++n;
                }
                c /= n;
                //computes a value fi along each axis
                double c_x, c_y, c_z;
                c_x = c[0];
                c_y = c[1];
                c_z = c[2];
                f0 += sign(c_x) * c_x * c_x;
                f1 += sign(c_y) * c_y * c_y;
                f2 += sign(c_z) * c_z * c_z;
            }

            //Size
            // compute the axis-aligned bounding box sizes
            /* double scale = -1;
    for (auto v : mesh.vertices())
    {
        double d = Distance(points[v], vec3(0, 0, 0));
        if (d >= scale)
            scale = d;
    }
    cout << "scale: " << scale << endl;*/

            //Scale
            Eigen::Vector3d b_min = mesh.bounds().min();
            Eigen::Vector3d b_max = mesh.bounds().max();
            Eigen::Vector3d D = b_max - b_min;
            double scale = 1 / D.maxCoeff();

            for (auto v : mesh.vertices())
            {
                points[v][0] = points[v][0] * scale * sign(f0);
                points[v][1] = points[v][1] * scale * sign(f1);
                points[v][2] = points[v][2] * scale * sign(f2);
            }

            ////holefilling(water-proof)
            //// find boundary halfedge
            //Halfedge h = find_boundary(mesh);

            //// fill hole
            //HoleFilling hf(mesh);
            //hf.fill_hole(h);

            //compute afterposscalar
            MatrixXd A_af(num_v, 3); //Get the matrix A_o
            i = 0;
            for (auto v : mesh.vertices())
            {
                A_af(i, 0) = points[v][0]; //x_coordinate
                A_af(i, 1) = points[v][1]; //y_coordinate
                A_af(i, 2) = points[v][2]; //z_coordinate
                i++;
            }
            Point afp = A_af.colwise().mean(); //! [barycenter]
            std::cout << "barycenter: " << A_af.colwise().mean() << std::endl;

            //translation
            //MatrixXd A_trans = A_af.rowwise() - A_af.colwise().mean();

            //Computing posscalar
            MatrixXd afcentered = A_af.rowwise() - A_af.colwise().mean();
            MatrixXd afcov = afcentered.adjoint() * afcentered;
            Eigen::EigenSolver<MatrixXd> afes(afcov);

            //MatrixXd afe_v = afes.pseudoEigenvectors();
            //Eigen::Vector3d afe1 = afe_v.row(0);

            //std::cout << "after vertices: " << mesh.n_vertices() << std::endl;
            //csvout << mesh.n_vertices() << "; ";
            //std::cout << "after-aabb size:" << mesh.bounds().size() << std::endl;
            //csvout << mesh.bounds().size() << ", ";
            //std::cout << "after-aabb center:" << mesh.bounds().center()
            //          << std::endl;
            //double afterd = Distance(mesh.bounds().center(), vec3(0, 0, 0));
            //std::cout << "after-aabb center distance:" << afterd << std::endl;
            //csvout << afterd << "; \n";
            //double afterposscalar =
            //    abs(afe1[0]) / abs(Distance(afe1, vec3(0, 0, 0)));
            //std::cout << "after posscalar: " << afterposscalar << std::endl;
            //csvout << afterposscalar << "; ";

            //mesh.write("scale.off");
            /***********************Step 3.2*******************************/
            auto S = surface_area(mesh);
            std::cout << "surface area: " << S << endl;
            csvout << S << ",";

            auto V = volume(mesh);
            auto C = (S * S * S) / (36 * M_PI * V * V);
            std::cout << "compactness: " << C << endl;
            csvout << C << ",";

            auto R = V / mesh.bounds().size();
            std::cout << "3D rectangularity: " << R << endl;
            csvout << R << ",";

            /*double diameter = 0;
            for (auto v1 : mesh.vertices())
            {
                for (auto v2 : mesh.vertices())
                {
                    double dist = Distance(points[v1], points[v2]);
                    if (dist > diameter)
                    {
                        diameter = dist;
                    }
                }
            }
            cout << "diameter: " << diameter << endl;
            csvout << diameter << ",";*/

            Eigen::VectorXcd eigenval = afes.eigenvalues();
            std::vector<double> com_eigenval = {
                eigenval(0).real(), eigenval(1).real(), eigenval(2).real()};
            int bigid = std::distance(
                com_eigenval.begin(),
                std::max_element(com_eigenval.begin(), com_eigenval.end()));
            int smallid = std::distance(
                com_eigenval.begin(),
                std::min_element(com_eigenval.begin(), com_eigenval.end()));
            /*cout << "x: " << eigenval(0).real() << "y: " << eigenval(1).real()
             << "z: "
             <<
            eigenval(2).real() << endl;
        cout << "biggest eigenval: " << bigid << endl;
        cout << "smallest eigenval: " << smallid << endl;*/
            double eccentricity =
                eigenval(bigid).real() / eigenval(smallid).real();
            std::cout << "eccentricity: " << eccentricity << endl;
            csvout << eccentricity << ",";

            //Vector<Scalar, 1> ax = normalize(Vector<Scalar, 1>(eccentricity));
            //cout << "ax: "<< ax << endl;

            //other descriptor
            std::vector<Point> p_arr;
            for (auto v : mesh.vertices())
            {
                p_arr.push_back(points[v]);
            }

            int data_count = 100; // 这里要是平方数不然bin算出来不对
            int bin = sqrt(data_count);

            std::vector<double> a3_descriptor;
            std::vector<double> d1_descriptor;
            std::vector<double> d2_descriptor;
            std::vector<double> d3_descriptor;
            std::vector<double> d4_descriptor;
            for (int i = 0; i < data_count; i++)
            {
                //A3
                vector<int> index_A3 = G_random(3, num_v);
                a3_descriptor.push_back(A3_angle(p_arr, index_A3));
                // D1
                vector<int> index_D1 = G_random(1, num_v);
                d1_descriptor.push_back(
                    d1_distance(p_arr, index_D1, p)); //p is baryceter
                //D2
                vector<int> index_D2 = G_random(2, num_v);
                d2_descriptor.push_back(d2_distance(p_arr, index_D2));
                //D3
                vector<int> index_D3 = G_random(3, num_v);
                d3_descriptor.push_back(d3_distance(p_arr, index_D3));
                //D4
                vector<int> index_D4 = G_random(4, num_v);
                d4_descriptor.push_back(d4_distance(p_arr, index_D4));
            }

            std::vector<double> a3_hist;
            a3_hist = hist(bin, a3_descriptor);
            std::vector<double> d1_hist;
            d1_hist = hist(bin, d1_descriptor);
            std::vector<double> d2_hist;
            d2_hist = hist(bin, d2_descriptor);
            std::vector<double> d3_hist;
            d3_hist = hist(bin, d3_descriptor);
            std::vector<double> d4_hist;
            d4_hist = hist(bin, d4_descriptor);

            std::cout << a3_hist << endl;
            std::cout << d1_hist << endl;
            std::cout << d2_hist << endl;
            std::cout << d3_hist << endl;
            std::cout << d4_hist << endl;
            csvout << a3_hist;
            csvout << d1_hist;
            csvout << d2_hist;
            csvout << d3_hist;
            csvout << d4_hist;
            csvout << "\n";

            //cycle control
            itera = cycle;
            cycle++;
            if (cycle == 261)
                cycle = 281;
            if (!autoscan || cycle == 401)
                scan = false;
        }
        if (!autoscan)
        {
            // Activate the viewer
            bool gui = true;
            MeshViewer viewer3("MeshViewer", 800, 600, gui);
            viewer3.load_mesh(filerealn);
            viewer3.run();
            viewer3.~MeshViewer();
            csvout.close();
            int iterend = itera;
            char target[10][20];
            bool awake_viewer = true;
            int numberofquery;
            std::cout << "Please enter K (How many meshes do you want to check for each query?): " << std::endl;
            std::cin >> numberofquery;
            for (itera; itera <= iterend; itera++)
            {
                //added
                int meshinfo_size = 57;
                ifstream file("csv/no_outlier115.csv", ios::in);
                vector<vector<string>> content;
                vector<string> row;
                vector<double> selected_mesh;
                vector<double> Eucstore;
                vector<vector<double>> Euclideandistancearray;
                vector<double> Cosstore;
                vector<vector<double>> Cosdistancearray;
                std::vector<double> av, bv, aw, bw, a3v, d1v, d2v, d3v, d4v,
                    a3w, d1w, d2w, d3w, d4w, ba3v, bd1v, bd2v, bd3v, bd4v, ba3w,
                    bd1w, bd2w, bd3w, bd4w;
                vector<double> EMDstore;
                vector<vector<double>> EMDdistancearray;
                string line, word, selected_meshclass;

                double Euclideandistance;
                bool findno = false;
                selected_mesh.push_back(0);
                selected_mesh.push_back(0);

                //std::cout << "iter" << itera << "started" << std::endl;
                csvout << itera << ",";
                //int wordcnt = 0, rowcnt = 0;
                if (file.is_open())
                {
                    while (getline(file, line))
                    {
                        row.clear();
                        stringstream str(line);
                        while (getline(str, word, ','))
                        {
                            if (!findno && word == std::to_string(itera))
                            {
                                getline(str, word, ',');
                                selected_meshclass = word;
                                for (int i = 0; i < meshinfo_size; i++)
                                {
                                    getline(str, word, ',');
                                    selected_mesh.push_back(stod(word));
                                }
                                findno = true;
                            }
                            else
                            {
                                row.push_back(word);
                                //wordcnt++;
                            }
                        }
                        content.push_back(row);
                        //rowcnt++;
                        //cout << "iter " << rowcnt << " wordcnt is " << wordcnt << endl;
                    }
                }
                else
                    std::cout << "Could not open the file\n";

                std::cout << "You have selected " << itera << ".off, it is a "
                          << selected_meshclass << std::endl;
                csvout << selected_meshclass << ",";

                /*for (int i = 0; i < content.size(); i++)
                cout << "content size, content[i] size: " << i
                     << ":" << content[i].size() << endl;*/
                /*for (int i = 0; i < content.size(); i++)
            {
                if (content[pointer].size() == 0)
                    continue;
                for (int j = 0; j < content[i].size(); j++)
                    std::cout << content[i][j] << ",";
            }*/

                //std::cout << "Mesh info: " << selected_mesh << std::endl;
                double Cossum = 0;

                //added
                for (int i = 0; i < content.size(); i++)
                {
                    /*std::cout << "Pointer is " << i << " ";*/
                    if (content[i].size() == 0)
                        continue;
                    double Eucfactor = 0;
                    double EMDEucfactor = 0;
                    double Coschild = 0;
                    double Cosparent = 1;
                    double Cosfactor1 = 0;
                    double Cosfactor2 = 0;
                    Eucstore.clear();
                    Cosstore.clear();
                    EMDstore.clear();
                    //std::vector<double> av, bv, aw, bw, a3v, d1v, d2v, d3v, d4v,a3w, d1w, d2w, d3w, d4w, ba3v, bd1v, bd2v, bd3v, bd4v, ba3w, bd1w, bd2w, bd3w, bd4w;
                    a3v.clear();
                    a3w.clear();
                    ba3v.clear();
                    ba3w.clear();
                    d1v.clear();
                    d1w.clear();
                    bd1v.clear();
                    bd1w.clear();
                    d2v.clear();
                    d2w.clear();
                    bd2v.clear();
                    bd2w.clear();
                    d3v.clear();
                    d3w.clear();
                    bd3v.clear();
                    bd3w.clear();
                    d4v.clear();
                    d4w.clear();
                    bd4v.clear();
                    bd4w.clear();
                    Eucstore.push_back(stod(content[i][0]));
                    Cosstore.push_back(stod(content[i][0]));
                    EMDstore.push_back(stod(content[i][0]));
                    for (int j = 2; j < meshinfo_size; j++)
                    {
                        //added
                        //EuclideanDistance
                        Eucfactor +=
                            pow((selected_mesh[j] - stod(content[i][j])), 2);
                        //CosDistance
                        Cosfactor1 = selected_mesh[j] * stod(content[i][j]);
                        Cosfactor2 = sqrt(pow(selected_mesh[j], 2) +
                                          pow(stod(content[i][j]), 2));
                        Coschild += Cosfactor1;
                        if (Cosfactor2 != 0)
                            Cosparent *= Cosfactor2;
                        //cout << content[i][j] << " ";
                        //EMD
                        if (j < 7)
                            EMDEucfactor += Eucfactor;
                        else if (j >= 7 && j < 17)
                        {
                            a3v.push_back(selected_mesh[j]);
                            ba3v.push_back(stod(content[i][j]));
                        }
                        else if (j >= 17 && j < 27)
                        {
                            d1v.push_back(selected_mesh[j]);
                            bd1v.push_back(stod(content[i][j]));
                        }
                        else if (j >= 27 && j < 37)
                        {
                            d2v.push_back(selected_mesh[j]);
                            bd2v.push_back(stod(content[i][j]));
                        }
                        else if (j >= 37 && j < 47)
                        {
                            d3v.push_back(selected_mesh[j]);
                            bd3v.push_back(stod(content[i][j]));
                        }
                        else
                        {
                            d4v.push_back(selected_mesh[j]);
                            bd4v.push_back(stod(content[i][j]));
                        }        
                    }
                    //Euc
                    Euclideandistance = sqrt(Eucfactor);
                    //std::cout << "the Euclidean distance to " << content[i][0] << ".off is" << Euclideandistance << std::endl;
                    Eucstore.push_back(Euclideandistance);
                    Euclideandistancearray.push_back(Eucstore);

                    //Cos
                    Cosstore.push_back(abs(1 - (Coschild / Cosparent)));
                    //std::cout << "Cosparent: " << Cosparent<< " Cosresult : " << 1 - (Coschild / Cosparent)<< std::endl;
                    Cosdistancearray.push_back(Cosstore);

                    //EMD here
                    for (int j = 0; j < 10; j++)
                    {
                        a3w.push_back(0);
                        ba3w.push_back(0);
                        d1w.push_back(0.1);
                        bd1w.push_back(0.1);
                        d2w.push_back(0.35);
                        bd2w.push_back(0.35);
                        d3w.push_back(0.35);
                        bd3w.push_back(0.35);
                        d4w.push_back(0.2);
                        bd4w.push_back(0.2);
                    }

                    //std::cout << "calculating EMD" << std::endl;
                    //std::cout << "calculating EMD" << std::endl;
                    EMDstore.push_back(log10(sqrt(EMDEucfactor)) +
                        log10(wasserstein(a3v, a3w, ba3v, ba3w)) +
                        log10(wasserstein(d1v, d1w, bd1v, bd1w)) +
                        log10(wasserstein(d2v, d2w, bd2v, bd2w)) +
                        log10(wasserstein(d3v, d3w, bd3v, bd3w)) +
                        log10(wasserstein(d4v, d4w, bd4v, bd4w)));

                    //std::cout << "the EMD to " << content[i][0] << ".off is"<< wasserstein(av, aw, bv, bw) << std::endl;
                    EMDdistancearray.push_back(EMDstore);
                }

                //querying result

                //Euclideandistance querying

                std::sort(Euclideandistancearray.begin(),
                          Euclideandistancearray.end(), sortcol);
                std::cout << "The most similar " << numberofquery
                          << " shapes to this " << selected_meshclass
                          << " at Euclidean Distance are : " << std::endl;
                std::cout << "     Class         No. Distance " << std::endl;
                //d = content.size(), c = Typecnt[?/20], s = numberofquery
                double TPcnt = 0, FPcnt = 0, FNcnt = 0, TNcnt = 0;
                for (int i = 0; i < numberofquery; i++)
                {
                    std::cout
                        << setw(10)
                        << Type[(int(Euclideandistancearray[i][0]) - 1) / 20]
                        << " ";
                    //added
                    //csvout << Type[(int(Euclideandistancearray[i][0]) - 1) / 20]<< ",";
                    for (int j = 0; j < Euclideandistancearray[i].size(); j++)
                    {
                        std::cout << setw(10) << Euclideandistancearray[i][j];
                        //added
                        //csvout << Euclideandistancearray[i][j] << ",";
                    }
                    //TP
                    if (selected_meshclass ==
                        Type[(int(Euclideandistancearray[i][0]) - 1) / 20])
                    {
                        std::cout << " TP";
                        TPcnt++;
                    }
                    std::cout << endl;
                }
                //d = content.size(), c = Typecnt[(int(distancearray[i][0]) - 1) / 20], s = numberofquery
                FPcnt = numberofquery - TPcnt;
                //FN = c - TP
                FNcnt = Typecnt[(itera - 1) / 20] - TPcnt;
                //TN = d - c - FP
                TNcnt = content.size() - Typecnt[(itera - 1) / 20] - FPcnt;
                if ((TPcnt + FPcnt + TNcnt + FNcnt) != content.size())
                    std::cout << "error";
                double EucPre = TPcnt / (TPcnt + FPcnt);
                double EucNPV = TNcnt / (TNcnt + FNcnt);
                double EucSen = TPcnt / (TPcnt + FNcnt);
                double EucSpe = TNcnt / (TNcnt + FPcnt);
                double EucAccuracy = (TPcnt + TNcnt) / content.size();
                std::cout << "TP count:" << TPcnt << std::endl;
                /*std::cout << TPcnt << ", " << FPcnt << ", " << FNcnt << ", "
                          << TNcnt << ", " << EucAccuracy << std::endl;
                std::cout << EucPre << ", " << EucNPV << std::endl;
                std::cout << EucSen << ", " << EucSpe << std::endl;*/
                csvout << EucAccuracy << "," << EucPre << "," << EucNPV << ","
                       << EucSen << "," << EucSpe << ",";

                //Cosine distance querying
                std::sort(Cosdistancearray.begin(), Cosdistancearray.end(),
                          sortcol);
                std::cout << "The most similar " << numberofquery
                          << " shapes to this " << selected_meshclass
                          << " at Cosine Distance are : " << std::endl;
                std::cout << "     Class              No.      Distance "
                          << std::endl;
                TPcnt = 0, FPcnt = 0, FNcnt = 0, TNcnt = 0;
                for (int i = Cosdistancearray.size() - 1;
                     i > Cosdistancearray.size() - numberofquery - 1; i--)
                {
                    std::cout << setw(10)
                              << Type[(int(Cosdistancearray[i][0]) - 1) / 20]
                              << " ";
                    //added
                    //csvout << Type[(int(Cosdistancearray[i][0]) - 1) / 20] << ",";
                    
                    for (int j = 0; j < Cosdistancearray[i].size(); j++)
                    {
                        std::cout << setw(15) << Cosdistancearray[i][j];
                        //added
                        //csvout << Cosdistancearray[i][j] << ","; 
                    }
                    //TP
                    if (selected_meshclass ==
                        Type[(int(Cosdistancearray[i][0]) - 1) / 20])
                    {
                        std::cout << " TP";
                        TPcnt++;
                    }
                    std::cout << endl;
                }
                //d = content.size(), c = Typecnt[(int(distancearray[i][0]) - 1) / 20], s = numberofquery
                FPcnt = numberofquery - TPcnt;
                //FN = c - TP
                FNcnt = Typecnt[(itera - 1) / 20] - TPcnt;
                //TN = d - c - FP
                TNcnt = content.size() - Typecnt[(itera - 1) / 20] - FPcnt;
                if (TPcnt + FPcnt + TNcnt + FNcnt != content.size())
                    std::cout << "error";
                double CosPre = TPcnt / (TPcnt + FPcnt);
                double CosNPV = TNcnt / (TNcnt + FNcnt);
                double CosSen = TPcnt / (TPcnt + FNcnt);
                double CosSpe = TNcnt / (TNcnt + FPcnt);
                double CosAccuracy = (TPcnt + TNcnt) / content.size();
                std::cout << "TP count:" << TPcnt << std::endl;
                /*std::cout << TPcnt << ", " << CosAccuracy << std::endl;
                std::cout << CosPre << ", " << CosNPV << std::endl;
                std::cout << CosSen << ", " << CosSpe << std::endl;*/
                csvout << CosAccuracy << "," << CosPre << "," << CosNPV << ","
                       << CosSen << "," << CosSpe << ",";

                //Euclidean + EMD distance querying
                std::sort(EMDdistancearray.begin(), EMDdistancearray.end(),
                          sortcol);
                std::cout << "The most similar " << numberofquery
                          << " shapes to this " << selected_meshclass
                          << " at EMD Distance are : " << std::endl;
                std::cout << "     Class         No. Distance " << std::endl;
                TPcnt = 0, FPcnt = 0, FNcnt = 0, TNcnt = 0;
                for (int i = 0; i < numberofquery; i++)
                {
                    std::cout << setw(10)
                              << Type[(int(EMDdistancearray[i][0]) - 1) / 20]
                              << " ";
                    //added
                    //csvout << Type[(int(EMDdistancearray[i][0]) - 1) / 20] << ",";
                    
                    for (int j = 0; j < EMDdistancearray[i].size(); j++)
                    {
                        std::cout << setw(10) << EMDdistancearray[i][j];
                        //added
                        //csvout << EMDdistancearray[i][j] << ",";
                    }
                    //TP
                    if (selected_meshclass == Type[(int(EMDdistancearray[i][0]) - 1) / 20])
                    {
                        std::cout << " TP";
                        TPcnt++;
                    }
                    std::cout << endl;
                }

                //d = content.size(), c = Typecnt[(int(distancearray[i][0]) - 1) / 20], s = numberofquery
                FPcnt = numberofquery - TPcnt;
                //FN = c - TP
                FNcnt = Typecnt[(itera - 1) / 20] - TPcnt;
                //TN = d - c - FP
                TNcnt = content.size() - Typecnt[(itera - 1) / 20] - FPcnt;
                if (TPcnt + FPcnt + TNcnt + FNcnt != content.size())
                    std::cout << "error";
                double EMDPre = TPcnt / (TPcnt + FPcnt);
                double EMDNPV = TNcnt / (TNcnt + FNcnt);
                double EMDSen = TPcnt / (TPcnt + FNcnt);
                double EMDSpe = TNcnt / (TNcnt + FPcnt);
                double EMDAccuracy = (TPcnt + TNcnt) / content.size();
                std::cout << "TP count:"<< TPcnt <<std::endl;
                /*std::cout << TPcnt << ", " << EMDAccuracy << std::endl;
                std::cout << EMDPre << ", " << EMDNPV << std::endl;
                std::cout << EMDSen << ", " << EMDSpe << std::endl;*/
                csvout << EMDAccuracy << "," << EMDPre << "," << EMDNPV << ","
                       << EMDSen << "," << EMDSpe << ",";

                //when the number of mesh equals to 1, select preferred distance
                if (awake_viewer)
                {
                    std::cout << "Which distance do you prefer? (e/c/m): "
                              << std::endl;
                    std::cout << "e for Euclidean; c for Cosine, m for EMD"
                              << std::endl;
                    std::cin >> examinput;
                    if (examinput == 'e')
                    {
                        for (int i = 0; i < numberofquery; i++)
                        {
                            std::sprintf(target[i], "%s%d%s", filepath,
                                         int(Euclideandistancearray[i][0]),
                                         filetype);
                        }
                    }
                    else if (examinput == 'c')
                    {
                        int meshtotal = Cosdistancearray.size() - 1;
                        for (int i = meshtotal;
                             i > meshtotal - numberofquery - 1; i--)
                        {
                            std::sprintf(target[meshtotal - i], "%s%d%s",
                                         filepath, int(Cosdistancearray[i][0]),
                                         filetype);
                            //std::cout << "target is " << target[meshtotal - i] << std::endl;
                        }
                    }
                    else if (examinput == 'm')
                    {
                        //EMD here if avaliable
                        for (int i = 0; i < numberofquery; i++)
                        {
                            std::sprintf(target[i], "%s%d%s", filepath,
                                         int(EMDdistancearray[i][0]), filetype);
                        }
                    }
                    else
                    {
                        std::cout << "illegal input!" << std::endl;
                        return 0;
                    }
                }

                //added
                content.clear();
                row.clear();
                selected_mesh.clear();
                Eucstore.clear();
                Cosstore.clear();
                EMDstore.clear();
                Euclideandistancearray.clear();
                Cosdistancearray.clear();
                EMDdistancearray.clear();
                csvout << std::endl;
                file.close();
                //std::cout << "iter" << itera << "ended" << std::endl;
                //csvout << "\n";
                if (itera == 24 || itera == 38 || itera == 57 || itera == 46 ||
                    itera == 356)
                    itera++;
                else if (itera == 260)
                    itera = 280;
            }
            if (awake_viewer)
            {
                bool gui = true;
                MeshViewer viewer("MeshViewer: 1st", 800, 600, gui);
                viewer.load_mesh(target[0]);
                viewer.run();
                viewer.~MeshViewer();

                MeshViewer viewer2("MeshViewer: 2nd", 800, 600, gui);
                viewer2.load_mesh(target[1]);
                viewer2.run();
                viewer2.~MeshViewer();

                MeshViewer viewer3("MeshViewer: 3rd", 800, 600, gui);
                viewer3.load_mesh(target[2]);
                viewer3.run();
                viewer3.~MeshViewer();
                exit(1);
            }
        }
        
        cin.get();
        return 0;
    }
}
