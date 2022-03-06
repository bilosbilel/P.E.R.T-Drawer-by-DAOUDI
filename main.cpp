
#include "resources.h"



using namespace std;
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")


//For buttons icons
HANDLE hImg;
HBITMAP hbmBall, hbmMask;
HICON folderico=LoadIcon(NULL,MAKEINTRESOURCE(BALLBMP));




/* enregistrement définir tache et etape */
struct Etape{
	int id;
    int dptot;
    int dptard;
    int marge;
    int X;
    int Y;
};
struct Tache{
    string name;
    int dure;
    int rang;
    list <Tache>antecedant;
    list <Tache> successeur;
    Etape etapeDebut;
    Etape etapeFin;
    bool Dessiner;
    
};



int i=0,j=0;

/* déclarations des reseux et chemin critiques*/
list<Tache> Projet;
Etape lastStep;
vector<Tache> cheminCritique ; 
list<Tache> FichierTaches; 
/*les boutons */
HWND btn1, btn2,btn3,btn4,lab1,button1,hButton;
/*les fenetres ou on ajout les composants */
HDC hDC;
PAINTSTRUCT Ps;
HWND hwnd, texta,textb ;

/*les methodes */
void AppeleDesMethode();
void DureeMinimale(HWND hwnd);
void trouverRang();
void DatePtot(); 
void DatePtard(); 
void MargeTotal(); 
void TrouverCordinations();
void etapeSuivant(); 
void changerEtapeAntece();
void NombreEtape();
void AddControls(HWND hwnd);
void desinGraph(HDC hdc);
void DeleteGraph(HDC hdc);
void rectangleFenetre(HDC hdc);
void QuitProgram(){PostQuitMessage(0);}


void DisplayFile(char *path){
	FILE *file;
	file =fopen(path,"rb");
	fseek(file,0,SEEK_END);
	int _size=ftell(file);
	rewind(file);
	char *data=new char(_size+1);
	fread(data,_size,1,file);
	data[_size] = '\0';
	
	SetWindowText(texta,data);
}

void ClearFile(){
	SetWindowText(texta,"contenu de graphe ici");
}

/* ****************************************************************************      */
/*   ALL NEEDED FUNCTIONS WILL BE DOWN HERE BUT NEED SOME EDIT  still need change    */
/* ****************************************************************************  *** */
bool ifIssetAllAntecedantsLevel(list<Tache> antecedants)
   {
        for (std::list<Tache>::iterator it=antecedants.begin(); it!=antecedants.end(); ++it)           
 {   if (it->rang == -1){return false;   }  }
   return true;
   }
  bool ifIssetAllAntecedantsEarliestStart(list<Tache> antecedants)
   {
   for (std::list<Tache>::iterator it=antecedants.begin(); it!=antecedants.end(); ++it)
   {
   if (it->etapeDebut.dptot == -1) {  return false;   }
   }
   return true;
   } 
  bool ifIssetAllSuccessorsLatestStart(list<Tache> successors)
   {
   for (std::list<Tache>::iterator it=successors.begin(); it!=successors.end(); ++it)
    {
    if (it->etapeDebut.dptard == BIGGEST || it->etapeDebut.dptard < it->etapeDebut.dptot){  return false; }
  }
   return true;
 }
void trouverRang()
        {
   int nbTache= Projet.size();
   int nbAddL = 0;         
   for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
   {
    if (it->antecedant.size() == 0)
    {
   it->rang = 1;
   nbAddL++;
   for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
    for (std::list<Tache>::iterator anteced1=it1->antecedant.begin(); anteced1!=it1->antecedant.end(); ++anteced1)
    if(anteced1->name==it->name)
  anteced1->rang=it->rang;                   
   }
  }
  while(nbAddL<nbTache) 
  {
   for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
    {         
    if(it->antecedant.size()!=0)
     {
    int rangmax = 0;
     {
    for (std::list<Tache>::iterator anteced=it->antecedant.begin(); anteced!=it->antecedant.end(); ++anteced)
    rangmax = max(rangmax,anteced->rang);
    it->rang = rangmax + 1; 
     nbAddL++; 
    for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
     for (std::list<Tache>::iterator anteced1=it1->antecedant.begin(); anteced1!=it1->antecedant.end(); ++anteced1)
     if(anteced1->name==it->name)
     anteced1->rang=it->rang;
    }
    }
}         
}
}
int getLevelMax()
{
  int levelMax = 0;
 for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
  {
     if(it->rang > levelMax) levelMax = it->rang;         
  }
return levelMax;        }
        /************* switch from switch to if *******************/
 void TrouverCordinations(int maxWidth) 
        {
            int coeff = 2*maxWidth / getLevelMax(); 
    for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
    {
    switch(it->rang)
    {
    case 1: 
     {it->etapeFin.X = coeff;
      it->etapeFin.Y = rand()*(700-50)/RAND_MAX +100;
     break;}
	case 2:{
         it->etapeFin.X = coeff*2;
        it->etapeFin.Y = rand()*(700-50)/RAND_MAX +50;
      break;}
    case 3:
        it->etapeFin.X = coeff*3;
        it->etapeFin.Y = rand()*(700-50)/RAND_MAX +50;
        break;
    case 4:
    it->etapeFin.X = coeff*4;
    it->etapeFin.Y = rand()*(700-50)/RAND_MAX +50;
    break;
    case 5:
	 it->etapeFin.X = coeff*5;
     it->etapeFin.Y = rand()*(700-50)/RAND_MAX +50;                     
      break;
    case 6:
     it->etapeFin.X = coeff*6;
     it->etapeFin.Y = rand()*(700-50)/RAND_MAX +50;
     break;
    case 7:
     it->etapeFin.X = coeff*7;
     it->etapeFin.Y = rand()*(700-50)/RAND_MAX +50;
    break;
      default:
     it->etapeFin.X = 750;
     it->etapeFin.Y = rand()*(700-50)/RAND_MAX +50;
    break;  }
       }
     }
 int findMaximum(vector<int> datesArray)
        {
            int maximum = datesArray[0];
            for (int i = 1; i < datesArray.size();i++)
            {    if(maximum<datesArray[i])   maximum = datesArray[i];  }
            return maximum;
        }
 int findMinimum(vector<int> datesArray)
        {
	int minimum = datesArray[0];
    for (int i = 1; i < datesArray.size();i++)
      { 
	     if(minimum>datesArray[i])       minimum = datesArray[i];
      }
    return minimum;        }
  
  void DatePtot()
    {            int DatePtotCalculer = 0; 
 for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
   {
   if (it->antecedant.size()==0)
      {
      it->etapeDebut.dptot = 0;
      DatePtotCalculer++;
  for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
     for (std::list<Tache>::iterator anteced1=it1->antecedant.begin(); anteced1!=it1->antecedant.end(); ++anteced1)
       if(anteced1->name==it->name)     anteced1->etapeDebut.dptot=0;
       }         
   }
     i=0;
    while(DatePtotCalculer<=Projet.size())
     {  
       for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
        {
   if(it->antecedant.size()!=0)
    {
   {
     vector<int> earliestStartDates ;
    for (std::list<Tache>::iterator anteced=it->antecedant.begin(); anteced!=it->antecedant.end(); ++anteced)
    {
     earliestStartDates.push_back(anteced->etapeDebut.dptot+anteced->dure);
     }
     it->etapeDebut.dptot= findMaximum(earliestStartDates);
     for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
    for (std::list<Tache>::iterator anteced1=it1->antecedant.begin(); anteced1!=it1->antecedant.end(); ++anteced1)
    if(anteced1->name==it->name)
    anteced1->etapeDebut.dptot=it->etapeDebut.dptot;                          
    DatePtotCalculer++;
   }
     }
       }             
         }
   vector<int> findProjctDelay ;
   for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
    {
    if(it->successeur.size()==0)  findProjctDelay.push_back(it->etapeDebut.dptot+it->dure);
    
    }
       lastStep.dptard=findMaximum(findProjctDelay);
       lastStep.dptot=findMaximum(findProjctDelay); 
    }
   void NombreEtape()
    {  
    int number=0;
   for (int i = 1; i <=getLevelMax() ; i++){
     for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
       {
         if(it->etapeDebut.id==-1 && it->rang==i)
     {
      number++;
      it->etapeDebut.id=number;
   for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1)
    {   
      if(it->name!=it1->name && it->etapeDebut.X ==it1->etapeDebut.X && it->etapeDebut.Y ==it1->etapeDebut.Y ) 
      it1->etapeDebut.id =it->etapeDebut.id;             
        }
      }
   } 
            } 
	lastStep.id=number+1;
        }
 bool ifIsAcheminCritiqueTask(Tache task)
        {
    for (i = 0; i <cheminCritique.size(); i++)
     if (cheminCritique[i].name == task.name)   return true;
            return false;
        }
 void etapeSuivant()
  {  
    for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
     {
    if(it->rang==1)
     {
       it->etapeDebut.X =50; 
       it->etapeDebut.Y =350;        
     }
    if(it->successeur.size()>1)
     {
      Etape sfstep = it->etapeFin;
    for (std::list<Tache>::iterator succes=it->successeur.begin(); succes!=it->successeur.end(); ++succes)
     {
      succes->etapeDebut.X = sfstep.X;
      succes->etapeDebut.Y = sfstep.Y;
    for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
    if(it1->name==succes->name)
	 {
      it1->etapeDebut.X=sfstep.X;
      it1->etapeDebut.Y=sfstep.Y;
	 }                   
    }
    }else
	{
     if(it->successeur.size()==1)
     {
      Etape sfstep = it->etapeFin;
      it->successeur.begin()->etapeDebut.X = sfstep.X;
      it->successeur.begin()->etapeDebut.Y = sfstep.Y;
    for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
     if(it1->name== it->successeur.begin()->name)
	 {
     it1->etapeDebut.X=sfstep.X;
     it1->etapeDebut.Y=sfstep.Y;
	 }                  
  }else{ 
     it->etapeFin.X = 700; 
     it->etapeFin.Y = 400;
     lastStep.X=it->etapeFin.X;
     lastStep.Y=it->etapeFin.Y;
     }
    }
   }
}
    void changerEtapeAntece()
        {
  for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
   {
  if(it->antecedant.size()>1)
    {
     Etape  step = it->etapeDebut;
   for (std::list<Tache>::iterator anteced=it->antecedant.begin(); anteced!=it->antecedant.end(); ++anteced)
    {
   anteced->etapeFin.X = step.X;
   anteced->etapeFin.Y = step.Y;
    for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
    if(it1->name== anteced->name)
	{
     it1->etapeFin.X=step.X;
     it1->etapeFin.Y=step.Y;
	 }                  
    }
   }
  }
}
   void DatePtard()
  {
  int DatePtardCalculer = 0;
  for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
   {
    if (it->successeur.size() == 0)
       {
        it->etapeDebut.dptard = lastStep.dptard- it->dure;
        DatePtardCalculer++;
     for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
	  for (std::list<Tache>::iterator succes1=it1->successeur.begin(); succes1!=it1->successeur.end(); ++succes1)
       if(succes1->name==it->name)    succes1->etapeDebut.dptard=lastStep.dptard- it->dure;                          
       } 
	}
    while(DatePtardCalculer<Projet.size())
    {
     for (std::list<Tache>::reverse_iterator it=Projet.rbegin(); it!=Projet.rend(); ++it)
      {
       if (it->successeur.size() != 0)
        {
         if (ifIssetAllSuccessorsLatestStart(it->successeur))
          {
          vector<int> latestStarttDates ;
          for (std::list<Tache>::iterator succes=it->successeur.begin(); succes!=it->successeur.end(); ++succes)
            {   
           latestStarttDates.push_back(succes->etapeDebut.dptard-it->dure);
            }                    
    it->etapeDebut.dptard = findMinimum(latestStarttDates);
    DatePtardCalculer++;
    for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
   {	
   for (std::list<Tache>::iterator succes1=it1->successeur.begin(); succes1!=it1->successeur.end(); ++succes1)
 {
  if(succes1->name==it->name) succes1->etapeDebut.dptard=it->etapeDebut.dptard;
 }
	}
   }
  }                  
 }
}
  for (std::list<Tache>::iterator it=FichierTaches.begin(); it!=FichierTaches.end(); ++it)
   {			 
    if(it->etapeDebut.dptard==BIGGEST)
     {        
      if(it->antecedant.size()==0)  it->etapeDebut.dptard=0;
    }
	}
   for (std::list<Tache>::iterator it=FichierTaches.begin(); it!=FichierTaches.end(); ++it)
   {
    vector<int> latestStarttDates ;
     for (std::list<Tache>::iterator succes1=it->successeur.begin(); succes1!=it->successeur.end(); ++succes1)
    {
    latestStarttDates.push_back(succes1->etapeDebut.dptard);
    int dpMin = findMinimum(latestStarttDates);               
	for (std::list<Tache>::iterator succes1=it->successeur.begin(); succes1!=it->successeur.end(); ++succes1)
     {
       succes1->etapeDebut.dptard=dpMin;
      for (std::list<Tache>::iterator it1=FichierTaches.begin(); it1!=FichierTaches.end(); ++it1)
      {				   
	if(succes1->name==it1->name)
       it1->etapeDebut.dptard=succes1->etapeDebut.dptard;
      }
	}
	} 
  }            
 }
 void MargeTotal() 
    {
   for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
   {
    it->etapeDebut.marge = it->etapeDebut.dptard -  it->etapeDebut.dptot;
    for (std::list<Tache>::iterator it1=Projet.begin(); it1!=Projet.end(); ++it1) 
    { for (std::list<Tache>::iterator succes1=it1->successeur.begin(); succes1!=it1->successeur.end(); ++succes1)
      if(succes1->name==it->name)
       succes1->etapeDebut.marge=it->etapeDebut.marge;
    for (std::list<Tache>::iterator anteced=it->antecedant.begin(); anteced!=it->antecedant.end(); ++anteced)
     if(anteced->name==it->name)   anteced->etapeDebut.marge=it->etapeDebut.marge;
	 }
    }
  }
bool taskNotExistIncheminCritique(Tache task,vector<Tache> cheminCritique)
{
    for (std::vector<Tache>::iterator it=cheminCritique.begin(); it!=cheminCritique.end(); ++it)
    if(it->name==task.name)return false;
    return true; 
}

vector<Tache> findcheminCritique()
  {
	vector<Tache> cheminCritique ;
    vector<Tache> tempPath ;
    bool add;
   for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
   {   
    add=false;
    if(it->etapeDebut.marge==0 )
    {                 
     if (it->successeur.size() != 0){
     for (std::list<Tache>::iterator succes1=it->successeur.begin(); succes1!=it->successeur.end(); ++succes1)
     if(succes1->etapeDebut.marge==0){
     add=true;                    }
	 }
     else add=true; 
    }
   if(add){ 
   tempPath.push_back(*it); 
    }            
  }
 for (i = 0; i <tempPath.size();i++)
  {
   for (j = 0; j < tempPath.size();j++)
    if(i!=j && tempPath[i].etapeDebut.X ==tempPath[j].etapeDebut.X && tempPath[i].etapeDebut.Y ==tempPath[j].etapeDebut.Y )
     {
    if(tempPath[i].etapeDebut.dptot>tempPath[j].etapeDebut.dptot)
     tempPath[i]=tempPath[j];
    else
     tempPath[j]=tempPath[i];
    }
    if(taskNotExistIncheminCritique(tempPath[i],cheminCritique))
     cheminCritique.push_back(tempPath[i]);           
   }            
   for (i = cheminCritique.size()-1; i >= 0; i--)
    {
     for (j = 0; j < i;j++)
     {
      if(cheminCritique[j].etapeDebut.dptot>cheminCritique[j+1].etapeDebut.dptot)
      {
      Tache tmp = cheminCritique[j + 1];
      cheminCritique[j + 1] = cheminCritique[j];
      cheminCritique[j] = tmp;
      }
     }
    }        
return cheminCritique;
}



template <class Container>
void partition(const std::string& str, Container& contenaire, char d = ' ')
{
    std::stringstream stringstream(str);
    std::string t;
    while (std::getline(stringstream, t, d)) {
        contenaire.push_back(t);}
}



void OpenFile(HWND hwnd){
	FichierTaches.clear();
	Projet.clear();
	 OPENFILENAME ofn;       // common dialog box structure
	TCHAR szFile[260] = { 0 };       // if using TCHAR macros
	char filename[250];
	
	// Initialize OPENFILENAME
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = filename;
	ofn.lpstrFile[0] = '\0'; 
	
	ofn.hwndOwner = hwnd;
	ofn.nMaxFile = 100;
	ofn.lpstrFilter = "Text\0*.TXT\0";
	ofn.nFilterIndex = 1;
	char buffer[250] = {0};
	if(GetOpenFileName(&ofn)){
	for(int i=0;i<sizeof(filename);i++)
	buffer[i]=filename[i];	
	}else{
	char text[20]={"text2.txt"} ;
	for(int i=0;i<sizeof(text);i++)
	buffer[i]= text[i];
	}
	
	map<string,Tache> taches;
    map<string,vector<string> > ante;
    map<string,vector<string> > succ;
    
	string line;
	LPCSTR settext;
	LPSTR gettext;
	std::string all_lines;
	ifstream file(buffer,ios::in);
     if(file)
    {           
     while(getline(file,line))
      {                         
        all_lines.append(line);
        all_lines.append("\n");
        vector<string> blocs;
        partition(line,blocs,';');
        vector<string> taskBloc ;
        taskBloc.push_back(blocs[0]);           
        vector<string>  antecedBloc;
        antecedBloc.push_back(blocs[1]);
        vector<string>  succesBloc;
        succesBloc.push_back(blocs[2]);
        vector<string> NameAndDuration;
        partition(taskBloc[0],NameAndDuration,',');
        vector<string> antecedant;
        partition(antecedBloc[0],antecedant,',');
        vector<string> successor;
        partition( succesBloc[0],successor,',');
        taches[NameAndDuration[0]].name = NameAndDuration[0];
        taches[NameAndDuration[0]].dure = atoi(NameAndDuration[1].c_str());
        taches[NameAndDuration[0]].rang=-1;
         if(antecedant[1]!="rien")
          {
   vector<string> tmp;
  for (int i = 1; i < antecedant.size();i++)  {    tmp.push_back (antecedant[i]);}
   ante[NameAndDuration[0]]=tmp; 
  }
   else
    {
   vector<string> tmp;
   tmp.clear();
   ante[NameAndDuration[0]]=tmp;
  }
   if (successor[1] != "rien")
   {
   vector<string> tmp;
  for (int i = 1; i < successor.size(); i++)
    {
   tmp.push_back (successor[i]);
     }
   succ[NameAndDuration[0]]=tmp;
  }
   else
     {
     vector<string> tmp;
     tmp.clear();
     succ[NameAndDuration[0]]=tmp;
     }        
    }
    for (std::map<string,Tache>::iterator it=taches.begin(); it!=taches.end(); ++it)
      {
     list<Tache> anteced ;
     vector<string> list = ante[it->first];
     for (int i = 0; i < list.size(); i++)
     {
      anteced.push_back(taches[list[i]]);
     }
     taches[it->first].antecedant=anteced;
     }
    for (std::map<string,Tache>::iterator it=taches.begin(); it!=taches.end(); ++it)
     {
      list<Tache> successor ;
      vector<string> list = succ[it->first];
      for (int i = 0; i < list.size(); i++)
      {
        successor.push_back(taches[list[i]]);
      }
      taches[it->first].successeur=successor;
      }
      for (std::map<string,Tache>::iterator it=taches.begin(); it!=taches.end(); ++it)
      {   Etape e;
          e.id=-1;
          e.X=-1;
          e.Y=-1;
          e.dptot=-1;
          e.dptard=BIGGEST;
          e.marge=-1;
          taches[it->first].etapeDebut=e;
          taches[it->first].etapeFin=e;
          FichierTaches.push_back(taches[it->first]);
        }                   
     			   settext = all_lines.c_str();
					SetWindowText(texta,settext);
			
	 
	 }else{
			SetWindowText(texta,"No file !");
		}
		
		file.close();
		
		Projet=FichierTaches;
		AppeleDesMethode();
		::EnableWindow(btn2,true);

}
/* This is where all the input to the window goes to */
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	switch(Message) {
		
		/* Upon destruction, tell the main thread to stop */
		case WM_COMMAND:{
			
			switch(wParam){
				case openfilebutton:{
					
					::MessageBox(hwnd,"Le fichier doit contenir : \n(ID , DUREE; list des ANTERIEURES; list des SUCCESEURES)\nEx : (A, 4;Ant, rien;Succ, B)","Information !",MB_ICONINFORMATION);
					OpenFile(hwnd);
					::EnableWindow(btn1,false);
					break;
				}					
				case clearfilebutton:
					ClearFile();
					DeleteGraph(hDC);
					::EnableWindow(btn1,true);
					::EnableWindow(btn2,false);
				break;
				case tracerfilebutton:{
				
					desinGraph(hDC);
					DureeMinimale(hwnd);
					::EnableWindow(btn1,false);				
				break;
				}
			
				case quit:
					QuitProgram();
				break;	
			}
			break;
		}
		case WM_PAINT: {
			hDC = BeginPaint(hwnd, &Ps);
			rectangleFenetre(hDC);
			
			break;
		}
		case WM_CREATE: {
			/*hbmBall = LoadBitmap(g_hInst, "BALLBMP");
			hbmMask = LoadBitmap(g_hInst, "MASKBMP");
			GetObject(hbmBall, sizeof(bm), &bm);
			*/
			SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX | SEM_NOALIGNMENTFAULTEXCEPT | SEM_FAILCRITICALERRORS);
			AddControls(hwnd);
			::EnableWindow(btn2,false);
			break;
		}
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
		
		
		/* All other messages (a lot of them) are processed using default procedures */
		default:
			return DefWindowProc(hwnd, Message, wParam, lParam);
	}
	return 0;
}

/* The 'main' function of Win32 GUI programs: this is where execution starts */
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEX wc; /* A properties struct of our window */
	HWND hwnd; /* A 'HANDLE', hence the H, or a pointer to our window */
	MSG Msg; /* A temporary location for all messages */

	
	/* zero out the struct and set the stuff we want to modify */
	memset(&wc,0,sizeof(wc));
	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.lpfnWndProc	 = WndProc; /* This is where we will send messages to */
	
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	
	/* White, COLOR_WINDOW is just a #define for a system color, try Ctrl+Clicking it */
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszClassName = "WindowClass";
	wc.hInstance        = hInstance;
    wc.hIcon            = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
    wc.hIconSm          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,"WindowClass","PERT",WS_VISIBLE|WS_OVERLAPPEDWINDOW | BS_BITMAP,
		0, /* x */
		0, /* y */
		1250, /* width */
		730, /* height */
		NULL,NULL,hInstance,NULL);
	
	
	if(hwnd == NULL) {
		MessageBox(NULL, "Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}

	/*
		This is the heart of our program where all input is processed and 
		sent to WndProc. Note that GetMessage blocks code flow until it receives something, so
		this loop will not produce unreasonably high CPU usage
	*/
	while(GetMessage(&Msg, NULL, 0, 0) > 0) { /* If no error is received... */
		TranslateMessage(&Msg); /* Translate key codes to chars if present */
		DispatchMessage(&Msg); /* Send it to WndProc */
	}
	return Msg.wParam;
}
void AddControls(HWND hwnd){

	button1 = CreateWindow(TEXT("BUTTON"), TEXT("open"), WS_VISIBLE | WS_CHILDWINDOW, 1050, 50, 175, 50, hwnd,(HMENU) 100, NULL, NULL);
//	SendMessage(GetDlgItem(hwnd,MODEL_SEARCH),BM_SETIMAGE, (WPARAM)IMAGE_ICON,(LPARAM)BALLBMP);
	
	
	btn1 =CreateWindow(TEXT("button"),TEXT("Ouvrir"),WS_VISIBLE|WS_CHILD ,1050,105,175,50,hwnd,(HMENU)openfilebutton ,NULL,NULL);
	btn2 =CreateWindow(TEXT("button"),TEXT("Tacer P.R.E.T"),WS_VISIBLE|WS_CHILD,1050,160,175,50,hwnd,(HMENU) tracerfilebutton,NULL,NULL);
	btn3 =CreateWindow(TEXT("button"),TEXT("Effacer"),WS_VISIBLE|WS_CHILD,1050,215,175,50,hwnd,(HMENU) clearfilebutton,NULL,NULL);
	btn4 =CreateWindow(TEXT("button"),TEXT("Quitter"),WS_VISIBLE|WS_CHILD,1050,275,175,50,hwnd,(HMENU) quit,NULL,NULL);
	texta =CreateWindow(TEXT("EDIT"),TEXT("contenu de graphe ici"),WS_VISIBLE|WS_CHILD|ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL,1050,335,175,200,hwnd,NULL,NULL,NULL);
	textb =CreateWindow(TEXT("EDIT"),TEXT("Chemin Critique | Duree"),WS_VISIBLE|WS_CHILD|ES_MULTILINE | WS_BORDER | ES_AUTOVSCROLL,1050,550,175,125,hwnd,NULL,NULL,NULL);
	CreateWindowW(L"static",L"Le Reseaux P.E.R.T va aparaite ci-dessus:",WS_VISIBLE|WS_CHILD,410,10,335,20,hwnd,(HMENU) 2,NULL,NULL);
}

			//dessigner methode
void rectangleFenetre(HDC hdc){
	HPEN hPenOld;
	HPEN hLinePen1;
	COLORREF qLineColor;
    qLineColor = RGB(128, 128,128);
    hLinePen1 = CreatePen(PS_SOLID, 1, qLineColor);
    
	MoveToEx(hDC, 10 , 150, NULL);
	hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
	RoundRect(hDC,10, 50, 1030, 680,20,20);
	
	
    SelectObject(hDC, hPenOld);
    DeleteObject(hLinePen1);
	
}
void desinGraph(HDC hdc){
HPEN hPenOld;
    HPEN hLinePen,hLinePen1;
    COLORREF qLineColor,qLineColor1;
    qLineColor = RGB(255, 10,10);
    qLineColor1 = RGB(25, 181,254);
    hLinePen = CreatePen(PS_SOLID, 4, qLineColor);
    hLinePen1 = CreatePen(PS_SOLID, 3, qLineColor1);
    
    hPenOld = (HPEN)SelectObject(hDC, hLinePen);
    int WindowHeight = 700 ; 
    int windowWidth = 900;
  for (std::list<Tache>::iterator it=Projet.begin(); it!=Projet.end(); ++it)
    {
    if(it->rang==1)  
     {
      it->Dessiner = true; 
      MoveToEx(hDC, 50 , 350, NULL);
      hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
      RoundRect(hDC,30,350,80,400,20,20);
      MoveToEx(hDC, 40 , 360, NULL);
      LineTo(hDC, 70,390);
      MoveToEx(hDC, 40 , 390, NULL);
      LineTo(hDC, 70,360);
      std::stringstream wsst,wssd,wssr,wssm;
      wsst << it->etapeDebut.id;
      wssd << it->etapeDebut.dptot;
      wssr << it->etapeDebut.dptard;
      wssm << it->etapeDebut.marge;
      LPCSTR pcst=wsst.str().c_str();
      TextOut(hDC,35,365,pcst,1);
      pcst=wssd.str().c_str();
      TextOut(hDC,50,350,pcst,2);
      pcst=wssr.str().c_str();
      TextOut(hDC,50,380,pcst,2);
      pcst=wssm.str().c_str();
      TextOut(hDC,65,365,pcst,1);
      if(ifIsAcheminCritiqueTask(*it))
       hPenOld = (HPEN)SelectObject(hDC, hLinePen);
      else
       hPenOld = (HPEN)SelectObject(hDC, hLinePen1);                     
      LineTo(hDC, it->etapeFin.X,it->etapeFin.Y);
      std::string a;
      std::stringstream wss;
      LPCSTR pcstrn;
	  a = it->name;
	  pcstrn = a.c_str();
	  TextOut(hDC,(50+it->etapeFin.X-30)/2,(350+it->etapeFin.Y)/2,pcstrn,2);
      wss << it->dure ;
      LPCSTR pcstr=wss.str().c_str();
      TextOut(hDC,(50+it->etapeFin.X)/2,(350+it->etapeFin.Y)/2,pcstr,2);
      hPenOld = (HPEN)SelectObject(hDC, hLinePen1);  
      }   
      else
        {
      it->Dessiner = true; 
      hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
      RoundRect(hDC,it->etapeDebut.X-25,it->etapeDebut.Y-25,it->etapeDebut.X+25,it->etapeDebut.Y+25,20,20);
      MoveToEx(hDC, it->etapeDebut.X-15, it->etapeDebut.Y-15, NULL);
      LineTo(hDC, it->etapeDebut.X+15,it->etapeDebut.Y+15);
      MoveToEx(hDC, it->etapeDebut.X-15 , it->etapeDebut.Y+15, NULL);
      LineTo(hDC, it->etapeDebut.X+15,it->etapeDebut.Y-15);      
      std::stringstream wssn,wssd,wssr,wssm;
      wssn << it->etapeDebut.id;
      wssd << it->etapeDebut.dptot;
      wssr << it->etapeDebut.dptard;
      wssm << it->etapeDebut.marge;
      LPCSTR pcst=wssn.str().c_str();
      TextOut(hDC,it->etapeDebut.X-20,it->etapeDebut.Y-10,pcst,1);
      pcst=wssd.str().c_str();
      TextOut(hDC,it->etapeDebut.X-5,it->etapeDebut.Y-25,pcst,2);
      pcst=wssr.str().c_str();
      TextOut(hDC,it->etapeDebut.X-5,it->etapeDebut.Y+5,pcst,2);
      pcst=wssm.str().c_str();
      TextOut(hDC,it->etapeDebut.X+10,it->etapeDebut.Y-10,pcst,2);
      if(ifIsAcheminCritiqueTask(*it))
       hPenOld = (HPEN)SelectObject(hDC, hLinePen);
      else
       hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
      MoveToEx(hDC,it->etapeDebut.X+15  ,it->etapeDebut.Y+15, NULL);
      LineTo(hDC, it->etapeFin.X-5,it->etapeFin.Y-15);
      std::string a;
      std::stringstream wss;
      LPCSTR pcstrn;
	  a = it->name;
	  pcstrn = a.c_str();
	  TextOut(hDC,(it->etapeDebut.X+it->etapeFin.X-30)/2,(it->etapeDebut.Y+it->etapeFin.Y)/2,pcstrn,2);
      wss << it->dure ;
      LPCSTR pcstr=wss.str().c_str();
      TextOut(hDC,(it->etapeDebut.X+it->etapeFin.X)/2,(it->etapeDebut.Y+it->etapeFin.Y)/2,pcstr,2);          
    } 
}
    hPenOld = (HPEN)SelectObject(hDC, hLinePen1);
    RoundRect(hDC,lastStep.X-25,lastStep.Y-25,lastStep.X+25,lastStep.Y+25,20,20);
    MoveToEx(hDC, lastStep.X-15,lastStep.Y-15, NULL);
    LineTo(hDC, lastStep.X+15,lastStep.Y+15);
    MoveToEx(hDC, lastStep.X-15 , lastStep.Y+15, NULL);
    LineTo(hDC, lastStep.X+15,lastStep.Y-15);
    std::stringstream wssn,wssd,wssr,wssm;
    wssn << lastStep.id;
    wssd << lastStep.dptot;
    wssr << lastStep.dptard;
    wssm << lastStep.marge;
    LPCSTR pcst=wssn.str().c_str();
    TextOut(hDC,lastStep.X-20,lastStep.Y-10,pcst,1);
    pcst=wssd.str().c_str();
    TextOut(hDC,lastStep.X-5,lastStep.Y-25,pcst,2);
    pcst=wssr.str().c_str();
    TextOut(hDC,lastStep.X-5,lastStep.Y+5,pcst,2);
    pcst=wssm.str().c_str();
    TextOut(hDC,lastStep.X+10,lastStep.Y-10,pcst,1);
    
    SelectObject(hDC, hPenOld);
    DeleteObject(hLinePen);
    
	::EnableWindow(btn1,false);
	::EnableWindow(btn2,true);
}
/*
void AppendText( const HWND &hwnd, TCHAR *newText )
{
    // get edit control from dialog
    HWND textb = GetDlgItem( hwnd, ID_TXT_B );
    // get new length to determine buffer size
    int outLength = GetWindowTextLength( textb ) + lstrlen(newText) + 1;
    // create buffer to hold current and new text
    TCHAR * buf = ( TCHAR * ) GlobalAlloc( GPTR, outLength * sizeof(TCHAR) );
    if (!buf) return;
    // get existing text from edit control and put into buffer
    GetWindowText( hwndOutput, buf, outLength );
    // append the newText to the buffer
    _tcscat_s( buf, outLength, newText );
    // Set the text in the edit control
    SetWindowText( hwndOutput, buf );
    // free the buffer
    GlobalFree( buf );
}
*/
void DureeMinimale(HWND hwnd){
LPCSTR id,id1;
string ch,original,saut,lines;
saut = "            |      \n";
lines = " ----------------------------------------\n";
std::stringstream dm;              
	for (int i = 0; i <cheminCritique.size() ; i++)   
	{  ch=ch+cheminCritique[i].name;
		if(i <cheminCritique.size())  ch=ch+"->";
	}
	id=ch.c_str();
    dm << lastStep.dptot;
	id1 = dm.str().c_str();
	int index = GetWindowTextLength (textb);
	SetFocus (textb);
	SendMessageA(textb, EM_SETSEL, (WPARAM)index, (LPARAM)index);
	//SendMessageA(textb, EM_REPLACESEL, 0, (LPARAM)original.c_str());
	SendMessageA(textb, EM_REPLACESEL, 0, (LPARAM)lines.c_str());
	SendMessageA(textb, EM_REPLACESEL, 0, (LPARAM)id);
	SendMessageA(textb, EM_REPLACESEL, 0, (LPARAM)saut.c_str());
	SendMessageA(textb, EM_REPLACESEL, 0, (LPARAM)id1);
	::EnableWindow(btn1,true);
}
void AppeleDesMethode(){
	trouverRang();
	DatePtot();     
	DatePtard();
    MargeTotal();
	TrouverCordinations(300);
	etapeSuivant();
	changerEtapeAntece();  
	NombreEtape();          
	cheminCritique = findcheminCritique();	
}
void DeleteGraph(HDC hdc){
	
	HBRUSH hBrush;
    // Create a solid blue brush.
    hBrush = CreateSolidBrush (RGB(255, 255, 255));
    // Associate the brush with the display device context.
    SelectObject (hdc, hBrush);
    // Draw a rectangle with blue background.
    Rectangle (hdc,  15,60,1000,670);
    // Create a hatch brush that draws horizontal red lines.
    hBrush = CreateHatchBrush(HatchStyleHorizontal, RGB(255, 255, 255));
    // Set the background color to yellow.
    SetBkColor(hdc, RGB(255, 255, 255));
    // Select the hatch brush background transparency based on user's input.
    if (true)
        // Make the hatch brush background transparent.
        // This displays the outer rectangle blue background.
        SetBkMode(hdc, TRANSPARENT);
    else
        // Make the hatch brush background opaque.
        // This displays the inner rectangle yellow background.
        SetBkMode(hdc, TRANSPARENT);
    // Associate the hatch brush with the current device context.
    //DeleteObject(hdc, hBrush);
    // Draw a rectangle with the specified hatch brush.
    HPEN hPen;
	HPEN hLinePen;
	COLORREF hLineColor;
    hLineColor = RGB(128, 128,128);
    hLinePen = CreatePen(PS_SOLID, 1, hLineColor);
    
	hPen = (HPEN)SelectObject(hDC, hLinePen);
	RoundRect(hDC,10, 50, 1030, 680,20,20);
	
	SelectObject(hDC, hPen);
    DeleteObject(hLinePen);
	FichierTaches.clear();
	Projet.clear();
	
	SetWindowText(textb,"Chemin Critique | Duree");
//	MoveToEx(hDC, -50 , -350, NULL);
}
/*
Coded By : BILEL DAOUDI 
LinkedIn : https://www.linkedin.com/in/daoudi-bilel
*/
