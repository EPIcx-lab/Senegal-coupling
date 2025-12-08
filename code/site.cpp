#include "site.h"
ifstream myStream;


inline bool exists_path (const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}




string path_creator(string basic_path, vector<pair<string, int> > folder_info)
{
    string mydir = basic_path;
    if (exists_path(mydir)==0) mkdir(mydir.c_str(), ACCESSPERMS);
    for (auto &element :  folder_info)
    {
        mydir += "/" + element.first + "_" + to_string(element.second);
        if (exists_path(mydir)==0) mkdir(mydir.c_str(), ACCESSPERMS);
    }
    return(mydir);
}


int main(int argc, char* argv[]){
    
    //================================ EXTERNAL PARAMETERS =====================================//
    int offset = stoi(argv[1]);
    int Nsim = stoi(argv[2]);
    
    string basic_path = "/data/giulia/aggregtaion_paper/simulations_site";
   
    /* GSL RANDOM NUMBER GENERATOR */
    
    unsigned long s = time(NULL)+offset*10000;
    gsl_rng *r = gsl_rng_alloc (gsl_rng_taus);
    if (r == NULL){
        printf("allocazione con gsl fallita\n");
        exit (EXIT_FAILURE);
    }
    gsl_rng_set (r, s);
    
    vector<double> r0= {1.1,1.5,3.};
    
    vector<string> methods= {argv[3]};
    
     
  
     string nodes;
     int cnt=0;
     nodes="/data/giulia/aggregtaion_paper/metapop_file/site/seeds.txt";
   
    
   
    vector<int> patches;
    patches.resize(0);
    patches.resize(n_seed);
    
    
    ifstream read_nodes(nodes);
     int n=0;
     
     if (read_nodes.good()){
     
     while (read_nodes >> n) {
     patches[cnt] = n;
     cnt++;
     }}
     
     else cout << "No info file found" << endl;
     read_nodes.close();
    
    
   
    //vector<int> patches= {95};
     // vector<int> patches= {38,135};
    
    for (unint metodo = 0; metodo < methods.size(); ++metodo)
    {
        for (unint h = 0; h < r0.size(); ++h)
        {
            for (unint seed= 0; seed < patches.size(); ++seed)
            {
                
                int patch= patches[seed];
                int r_zero= r0[h];
                int inf_0=10;
                string  met=methods[metodo];
                
                int  t_a[n_patch];
                
                unint  I_tot[n_patch];
                memset(I_tot, 0, sizeof I_tot);
                
                int  t_p[n_patch];
                double  tree[n_patch][n_patch];
                memset(tree, 0, sizeof tree);
                
                
                if(r0[h]==1.5){r_zero=2;}
                
                string output_path;
                int len2;
                int len3;
                int len4;
                int t_star=0;
                int t_star2=0;
                int t_star3=0;
                
                vector<int> patches_inf;
                
                int mmm=0;
                if(met=="Result_L1") { mmm=0;}
                if(met=="Result_C1") { mmm=1;}
                 
                vector<pair<string,int> > folders {make_pair("Metodo",mmm),make_pair("Ro",r_zero), make_pair("seed", patch)};

                
                for (int isim = 0; isim < Nsim; ++isim){
                    
                    double somma=0;
                    
                    memset(tree, 0, sizeof tree);
                    while(somma==0){
                        
                        int len=0;
                        len2=0;
                        len3=0;
                        len4=0;
                        int  map_S[n_patch][TIME+1];
                        memset(map_S, 0, sizeof map_S);
                        int  map_E[n_patch][TIME+1];
                        memset(map_E, 0, sizeof map_E);
                        int  map_I[n_patch][TIME+1];
                        memset(map_I, 0, sizeof map_I);
                        int  map_R[n_patch][TIME+1];
                        memset(map_R, 0, sizeof map_R);
                        
                        memset(tree, 0, sizeof tree);
                        
                        
                        patches_inf.resize(0);
                        patches_inf.resize(n_patch);
                        
                        
                        
                        memset(I_tot, 0, sizeof I_tot);
                        
                        memset(t_a, 0, sizeof t_a);
                        
                        memset(t_p, 0, sizeof t_p);
                        
                        unint   I_peak[n_patch];
                        memset(I_peak, 0, sizeof  I_peak);
                        
                        
                        /* alloco memoria e definisco variabili*/
                        
                        int  t0=0;
                        
                        
                        t_star=0;
                        
                        t_star2=0;
                        t_star3=0;
                        
                        
                        
                        double beta=(double)(r0[h]*mu);
                        int giorni=0;
                        
                        int mese=8;
                        
                        int mesi=0;
                        int epidemic=1;
                        nodo D_map[n_patch];
                        
                        int  I_patch=0;
                        int  I_patch_u=0;
                        
                        while(epidemic!=0){
                            
                            /*read pop distribution and adjaent matrix */
                            if(mese==0 || mese==2 || mese==4|| mese==6|| mese==7|| mese==9|| mese==11){giorni=31;}
                            if(mese==1){giorni=28; }
                            if(mese==3||mese==10||mese==5||mese==8){giorni=30;}
                            
                            string  network;
                            network="/data/giulia/aggregtaion_paper/metapop_file/site/degree_pop_nodi"+to_string(mese)+"_"+met+".txt";
                            
                            /*degree tiene in considerazione anche selfloop*/
                            ifstream read_population(network);
                            int  subpop;
                            int  degree_in;
                            int  degree_out;
                            int  count=0;
                            memset(D_map, 0, sizeof D_map);
                            if (read_population.good()){
                                while (read_population >> subpop >>degree_out>>degree_in) {
                                    D_map[count].n_subpop = subpop;
                                    D_map[count].degree_out= degree_out;
                                    D_map[count].degree_in= degree_in;
                                    
                                    count++;
                                }}
                            
                            else cout << "No info file found 1" << endl;
                            read_population.close();
                            
                            /*inizializzo*/
                            
                            for(int i = 0; i <n_patch; ++i){
                                map_S[i][t0]=(int)D_map[i].n_subpop;
                                map_E[i][t0]=0;
                                map_I[i][t0]=0;
                                map_R[i][t0]=0;}
                            
                            map_I[patch][t0]=inf_0;
                            map_S[patch][t0]= (int)(map_S[patch][t0]-map_I[patch][t0]);
                            
                            
                            
                            string  matrix;
                            vector<vector<int>> contact_list_in; //adjacency lists
                            vector<vector<double>> prob_list_in; //diffusion lists
                            contact_list_in.resize(0);
                            contact_list_in.resize(n_patch+1);
                            prob_list_in.resize(0);
                            prob_list_in.resize(n_patch+1);
                            
                            vector<vector<int>> contact_list_out; //adjacency lists
                            vector<vector<double>> prob_list_out; //diffusion lists
                            contact_list_out.resize(0);
                            contact_list_out.resize(n_patch+1);
                            prob_list_out.resize(0);
                            prob_list_out.resize(n_patch+1);
                            double prob_stay[n_patch];
                            memset(prob_stay, 0, sizeof prob_stay);
                            matrix="/data/giulia/aggregtaion_paper/metapop_file/site/"+met+"_"+ to_string(mese) +"_metapop.txt";
                            ifstream read_matrix(matrix);
                            
                            int  n;
                            int  m;
                            float prob;
                            if (read_matrix.good()){
                                while (read_matrix >> n>>m>> prob) {
                                    
                                    if(n!=m){
                                        contact_list_out.at(n).push_back(m);
                                        prob_list_out.at(n).push_back(prob);
                                        
                                        contact_list_in.at(m).push_back(n);
                                        prob_list_in.at(m).push_back(prob);
                                    }
                                    if(n==m){
                                        prob_stay[n]=prob;
                                    }}
                            }
                            
                            
                            else cout << "No info file found 2" << endl;
                            read_matrix.close();
                            
                            int day=0;
                            
                            for(int giorno=0;giorno<giorni;++giorno){
                                day=giorno+mesi;
                                int scenari_tot=0;
                                
                                
                                for(int i = 0;i<n_patch; ++i){
                                    
                                    int new_time = (day + 1);
                                    const unint susc = map_S[i][day];
                                    const unint exposed = map_E[i][day];
                                    const unint infect = map_I[i][day];
                                    const unint recovered = map_R[i][day];
                                    const int N_sub = susc + exposed+ infect + recovered;
                                    float lambda_stay=0;
                                    
                                    float  lambda_v[D_map[i].degree_in-1];
                                    memset(lambda_v, 0, sizeof lambda_v);
                                    float  lambda_r[D_map[i].degree_out-1];
                                    memset(lambda_r, 0, sizeof lambda_r);
                                    
                                    int    N_sub_j=0;
                                    float  N_sub_eff=0;
                                    float  N_sub_eff_j=0;
                                    float  I_sub_eff_j=0;
                                    float  I_sub_eff=0;
                                    
                                    I_tot[i]+=infect;
                                    
                                    
                                    N_sub_eff = prob_stay[i]*N_sub;
                                    I_sub_eff = prob_stay[i]*infect;
                                    
                                    for(int j=0;j<D_map[i].degree_in-1;++j){
                                        int node= contact_list_in[i][j];
                                        if(node!=i){
                                            
                                            N_sub_eff += (prob_list_in[i][j]*D_map[node].n_subpop);
                                            I_sub_eff += (prob_list_in[i][j]*map_I[node][day]);
                                        } }
                                    
                                    for(int j=0;j<D_map[i].degree_in-1;++j){
                                        int node= contact_list_in[i][j];
                                        if(node!=i){
                                            lambda_v[j]=(double)((prob_stay[i]*prob_list_in[i][j]*map_I[node][day])/N_sub_eff);
                                        } }
                                    int test=0;
                                    for(int j=0;j<D_map[i].degree_out-1;++j){
                                        
                                        int node= contact_list_out[i][j];
                                        if(node!=i){
                                            N_sub_j=D_map[node].n_subpop;
                                            N_sub_eff_j =prob_stay[node]*N_sub_j ;
                                            I_sub_eff_j =prob_stay[node]*map_I[node][day];
                                            
                                            for(int k=0;k<D_map[node].degree_in-1;++k){
                                                int node2= contact_list_in[node][k];
                                                int N_sub_k=D_map[node2].n_subpop;
                                                N_sub_eff_j += (prob_list_in[node][k]*N_sub_k);
                                                I_sub_eff_j += (prob_list_in[node][k]*map_I[node2][day]);
                                            }
                                            lambda_r[j]=(double)((prob_list_out[i][j]*I_sub_eff_j)/N_sub_eff_j);
                                            
                                            test+=1;
                                        }}
                                    
                                    
                                    lambda_stay=(double)((prob_stay[i]*prob_stay[i]*infect)/N_sub_eff);
                                    
                                    
                                    /*reaction processes*/
                                    
                                    int new_infected =  0;
                                    int new_exposed =  0;
                                    int new_recovered = 0;
                                    
                                    scenari_tot=D_map[i].degree_in+D_map[i].degree_out;
                                    
                                    unint dif[scenari_tot];
                                    double diffusion_rates[scenari_tot];
                                    
                                    memset(dif, 0, sizeof dif);
                                    memset(diffusion_rates, 0, sizeof diffusion_rates);
                                    
                                    
                                    int count=1;
                                    double f=0;
                                    
                                    for(int k=1;k<D_map[i].degree_in;++k){
                                        diffusion_rates[count]=beta*lambda_v[k-1];
                                        f+=diffusion_rates[count];
                                        count+=1;
                                    }
                                    
                                    for(int k=D_map[i].degree_in;k<scenari_tot-1;++k){
                                        diffusion_rates[count]=(beta*lambda_r[k-D_map[i].degree_in]);
                                        f+=diffusion_rates[count];
                                        count+=1;
                                    }
                                    
                                    diffusion_rates[scenari_tot-1]=(beta*lambda_stay);
                                    f+=diffusion_rates[scenari_tot-1];
                                    
                                    diffusion_rates[0]=(1-f);
                                    
                                    
                                    /*float lambda=0;
                                     for(int k=0;k<D_map[i].degree_in-1;++k){
                                     
                                     lambda+=(beta*lambda_v[k]);
                                     }
                                     
                                     for(int k=0;k<D_map[i].degree_out-1;++k){
                                     lambda+=(beta*lambda_r[k]);
                                     }
                                     
                                     lambda+=(beta*lambda_stay);*/
                                    
                                    if(susc!=0){
                                        gsl_ran_multinomial(r,scenari_tot,susc,diffusion_rates,dif);
                                        
                                        
                                        for(int d=1;d<scenari_tot;++d){
                                            new_exposed+=dif[d]; }
                                    }
                                    
                                    
                                    /*for(int i=1;i<=scenari_tot;++i){
                                     lambda+=diffusion_rates[i];
                                     }*/
                                    
                                    /*lambda = (float)(infect*beta)/N_sub;*/
                                    
                                    if(exposed!=0){
                                        new_infected=(int)gsl_ran_binomial(r,epsilon, exposed);
                                    }
                                    
                                    if(infect!=0){
                                        new_recovered=(int)gsl_ran_binomial(r, mu, infect);
                                    }
                                    /* if(susc!=0){
                                     new_exposed=(int)gsl_ran_binomial(r, lambda, susc);
                                     }*/
                                    map_S[i][new_time] = (susc- new_exposed);
                                    map_E[i][new_time] = (exposed+new_exposed)-new_infected;
                                    map_I[i][new_time] = (infect+new_infected)-new_recovered;
                                    map_R[i][new_time] = (recovered+new_recovered);
                                    
                                    double cazzo=0;
                                    
                                    if((map_E[i][new_time]!=0)&(map_S[i][day]==D_map[i].n_subpop)){
                                        t_a[i]=new_time;
                                        
                                        if(i<6){I_patch_u+=1;}
                                        patches_inf[len]=i;
                                        
                                        len+=1;
                                        I_patch+=1;
                                        
                                        if(patch>=6){
                                            if(i<6){
                                                if(t_star2==0){
                                                    t_star2=new_time;
                                                } }  }
                                        
                                        for(int l=1;l<scenari_tot-1;++l){
                                            if(dif[l]!=0){
                                                cazzo+=1;
                                            }}
                                        
                                        for(int l=1;l<scenari_tot-1;++l){
                                            if(dif[l]!=0){
                                                
                                                if(l<D_map[i].degree_in){
                                                    int node= contact_list_in[i][l-1];
                                                    tree[node][i]=(1./cazzo);}
                                                if(l>=D_map[i].degree_in) {
                                                    int node= contact_list_out[i][l-D_map[i].degree_in];
                                                    tree[node][i]=(1./cazzo);
                                                }
                                            } }
                                    }
                                    
                                    if(I_tot[i]>I_peak[i]){
                                        I_peak[i]=I_tot[i];
                                        t_p[i]=new_time;}
                                    
                                }  // chiudepatch i
                                
                                if((I_patch>34) & (len2==0)){
                                    if(t_star==0){
                                        t_star=day+1;
                                        len2=len;}}
                                
                                
                                if((I_patch_u==6) & (t_star3==0)){
                                    t_star3=day+1;
                                }
                                
                                int inf=0;
                                
                                for(int i=0;i<n_patch;++i){
                                    inf+=(map_I[i][day+1]+map_E[i][day+1]);
                                }
                                
                                if(inf==0){
                                    epidemic=0;
                                }
                                if(day==TIME){
                                    epidemic=0;
                                    
                                }
                            }/*chiude giorno*/
                            
                            mesi+=giorni;
                            mese+=1;
                            if(mese==12){mese=0;}
                            
                        }/*chiude epidemic*/
                        
                        for(int j=0;j<n_patch;++j){
                            if((j!=patch) & (tree[patch][j]!=0)){
                                somma+=1;
                            } }
                    }/*somma*/
                    
                    
		     output_path = path_creator(basic_path, folders);
                    
		     
		       ofstream myfileb;
                     
                     myfileb.open(output_path +"/offset_"+to_string(offset)+"_"+ to_string(isim) + "_arrival.txt");
                     if (myfileb.is_open()){
                     for(int j=0;j<n_patch;++j){
                     myfileb <<j<<" "<< t_a[j]<<"\n";
                     }
                     myfileb.close();
                     }  else cout <<"Unable to open file";
                     
                     
                     
                 
                   
	             ofstream myfilea;
                     myfilea.open(output_path + "/offset_"+to_string(offset) + "_run" +to_string(isim) + "patches_inf.txt");
                     if (myfilea.is_open()){
                     for(int j=0;j<n_patch;++j){
                     myfilea <<patches_inf[j]<<"\n";
                     }
                     myfilea.close();
                     }  else cout <<"Unable to open file";
                       
		     /*
		    
   ofstream myfilez;
                    
                    myfilez.open(output_path +"/offset_"+to_string(offset)+"_"+ to_string(isim)+ "_tree_1000.txt");
                    if (myfilez.is_open()){
                        for(int i=0;i<n_patch;++i){
                            for(int j=0;j<n_patch;++j){
                                
                                if(tree[i][j]!=0.0){
                                    myfilez<<i<<" "<<j<<" "<<tree[i][j]<<"\n";
                                } } }
                        myfilez.close();
                    }  else cout <<"Unable to open file";
                   
                   
		     */

		     
                      }/*chiude run*/
                    
              
                
            }// chiude ro
        } // chiudo seed
    } // chiudo method
    
    
    
}/*chiude main*/

