#include "functions.h"

int main() {
//    std::cout << proportion_to_0_1(200) << "\n";
//    std::cout << proportion_to_0_255(0.5) << "\n";
//    float aux = proportion_to_0_1(200);
//    std::cout << aux << " aux:  \n";
//    float pixel = 0.5 * pow(aux, 0.9);
//    std::cout << pixel << " pixel depois do calculo\n";
//    pixel = proportion_to_0_255(pixel);
//    std::cout << pixel << " pixel passado para 255\n";

    int imgChoice, choice, p, k;
    const char *file_name = nullptr;
    char *new_file_name = (char *)"/home/caiotavares/Documents/pdi/conteudo_04/imge-input.pgm";

    // O usuário escolhe a imagem que deseja usar:
    cout << "\nQual imagem deseja usar?";
    cout << "\n1.lena256.pgm";
    cout << "\n2.ctskull-256.pgm";
    cout << "\n3.quadrados.pgm";
    cout << "\n4.tungsten_original.pgm";
    cout << "\n5.blurring.pgm";
    cout << "\n6.blurry_moon.pgm";
    cout <<  "\n\nEscolha #/> ";
    cin >> imgChoice;

    if (imgChoice == 1) {
        file_name = "/home/caiotavares/Documents/pdi/conteudo_04/images/lena-256.pgm";
    } else if (imgChoice == 2) {
        file_name = "/home/caiotavares/Documents/pdi/conteudo_04/images/ctskull-256.pgm";
    } else if (imgChoice == 3) {
        file_name = "/home/caiotavares/Documents/pdi/conteudo_04/images/quadrados.pgm";
    } else if (imgChoice == 4) {
        file_name = "/home/caiotavares/Documents/pdi/conteudo_04/images/tungsten_original.pgm";
    } else if (imgChoice == 5) {
        file_name = "/home/caiotavares/Documents/pdi/conteudo_04/images/blurring.pgm";
    } else if (imgChoice == 6) {
        file_name = "/home/caiotavares/Documents/pdi/conteudo_04/images/blurry_moon.pgm";
    }


    Image image = read_image((char *) file_name);

    //Now print the array to see the result
    for(int row = 0; row < image.numrows; ++row) {
        for(int col = 0; col < image.numcols; ++col) {
            cout << image.array[row][col] << " ";
        }
        cout << endl;
    }

    cout << "\n1.Escurecer imagem";
    cout << "\n2.Negativo";
    cout << "\n3.Clarear";
    cout << "\n4.Girar 90º";
    cout << "\n5.Girar -90º";
    cout << "\n6.Girar 180º";
    cout << "\n7.Binarização";
    cout << "\n8.Espelhamento horiontal a direita FIX";
    cout << "\n9.Binarização de quatro fatores";
    cout << "\n10.Binarização de três fatores";
    cout << "\n11.Subtração";
    cout << "\n12.Aumentar resolução (x4)";
    cout << "\n13.Diminuir resolução (/4)";
    cout << "\n14.Equalização de histograma";
    cout << "\n15.Equalização local de histograma";
    cout << "\n16.Bluring";
    cout << "\n17.Laplaciano de máscara diagonal";
    cout << "\n18.Laplaciano de máscara de vizinhança-8";
    cout <<  "\n\nEscolha #/> ";
    cin >> choice;

    if (choice == 1) {
        cout << "[0 - 255] #/> ";
        cin >> p;
        while (p < 0 || p > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> p;
        }
        image = darken(image, p);
    } else if (choice == 2) {
        image = negative(image);
    } else if (choice == 3) {
        cout << "[0 - 255] #/> ";
        cin >> p;
        while (p < 0 || p > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> p;
        }
        image = whiten(image, p);
    } else if (choice == 4) {
        image = turn_plus_90(image);
    } else if (choice == 5) {
        image = turn_minus_90(image);
    } else if (choice == 6) {
        image = turn_plus_180(image);
    } else if (choice== 7) {
        cout << "[0 - 255] K: #/> ";
        cin >> k;
        while (k < 0 || k > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> k;
        }
        image = binarizing(image, k);
    } else if (choice == 8) {
        image = horizontal_mirror_right(image);
    } else if (choice == 9) {
        int a, b, s_sup, s_inf;
        //A
        cout << "[0 - 255] A: #/> ";
        cin >> a;
        while (a < 0 || a > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> a;
        }
        cout << "[0 - 255] B: #/> ";
        cin >> b;
        while (b < 0 || b > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> b;
        }
        cout << "[0 - 255] s superior (limite superior da binarização): #/> ";
        cin >> s_sup;
        while (s_sup < 0 || s_sup > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> s_sup;
        }
        cout << "[0 - 255] s inferior (limite inferior da binarização): #/> ";
        cin >> s_inf;
        while (s_inf < 0 || s_inf > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> s_inf;
        }
        image = variables_binarize_4_factors(image, a, b, s_sup, s_inf);
    } else if (choice == 10) {
        int a, b, s_sup;
        cout << "[0 - 255] A: #/> ";
        cin >> a;
        while (a < 0 || a > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> a;
        }
        cout << "[0 - 255] B: #/> ";
        cin >> b;
        while (b < 0 || b > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> b;
        }
        cout << "[0 - 255] s superior (limite superior da binarização): #/> ";
        cin >> s_sup;
        while (s_sup < 0 || s_sup > 256) {
            cerr << "O valor deve estar entre 0 e 255.\n ###/>";
            cin >> s_sup;
        }
        image =  variables_binarize_3_factors(image, a, b, s_sup);
    } else if (choice == 11) {
        float c, a;
        cout << "c: #/> ";
        cin >> c;
        cout << "a: #/> ";
        cin >> a;
        image = subtraction(image, c, a);
    } else if (choice == 12) {
        image = resolution_x4(image);
    } else if (choice == 13) {
        image = poor_quality(image);
    } else if (choice == 14) {
        image = histogram_equalization(image);
    } else if (choice == 15) {
        image = local_histogram_equalization(image);
    } else if (choice == 16) {
        image = blurring(image);
    } else if (choice == 17) {
        image = laplace(image);
    } else if (choice == 18) {
        image = laplace2(image);
    }

    for(int row = 0; row < image.numrows; ++row) {
        for(int col = 0; col < image.numcols; ++col) {
            cout << image.array[row][col] << " ";
        }
        cout << endl;
    }

    cout << "\nSalvar";
    cout << "\n1.Salvar em linha única";
    cout << "\n1.Salvar em matriz";

    cout <<  "\n\nEscolha #/> ";
    cin >> choice;

    if (choice == 1) write_pgm(image.array, image.numrows, image.numcols, new_file_name);
    else if (choice ==2 ) write_pgm_matrix(image.array, image.numrows, image.numcols, new_file_name);

    // free the memory
    for (int i = 0; i < image.numrows; i++) {
        delete[] image.array[i];
    }
    delete[] image.array;

    return 0;
}
