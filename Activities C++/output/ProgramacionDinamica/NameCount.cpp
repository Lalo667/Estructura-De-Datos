    #include <iostream>
    using namespace std;

    main(){
        string name;
        int large;
        int conta = 0;
        int ca=0;
        int ce=0;
        int ci=0;
        int co=0;
        int cu=0;
        int blank=0;

        cout <<"Ingrese su nombre: ";
        getline(cin, name);
        large = name.length();
  
        for (int i =0; i < large; i++ ){
            if (name[i]=='a'|| name[i] =='A') {
              ca += 1;
        } 
            if (name[i]=='e'|| name[i] =='E') {
              ce += 1;
        } 
            if (name[i]=='i'|| name[i] =='I') {
              ci += 1;
        } 
            if (name[i]=='o'|| name[i] =='O') {
              co += 1;
        } 
            if (name[i]=='u'|| name[i] =='U') {
              cu += 1;
        } 
            if (name[i] == ' ') {
              blank += 1;
        } 
    }
    large  -= blank;
    cout << "Nombre Ingresado: " << name << "\n";
    cout << name << "Tiene: " << large << "letras\n";
    cout << "a: " << ca << "\n";
    cout << "e: " << ce << "\n";
    cout << "i: " << ci << "\n";
    cout << "o: " << co << "\n";
    cout << "u: " << cu << "\n";
    cout << "Espacios en blanco: " << blank << "\n";
    return 0;
            
    }
    

