    
  #include "site.h"
ifstream myStream;


inline bool exists_path (const std::string& name)
{
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}


// Creates nested directory structure based on folder information
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
        printf("GSL allocation failed\n");
        exit (EXIT_FAILURE);
    }
    gsl_rng_set (r, s);
    
    // Vector of R0 values (basic reproduction number)
    vector<double> r0= {1.1,1.5,3.};
    
    // Methods vector (D, L, or C coupling matrix)
    vector<string> methods= {argv[3]};
    
    // Read seed nodes from file
    string nodes;
    int cnt=0;
    nodes="/data/giulia/aggregtaion_paper/metapop_file/site/seeds.txt";
   
    // Initialize patches (municipalities) vector
    vector<int> patches;
    patches.resize(0);
    patches.resize(n_seed);
    
    
    ifstream read_nodes(nodes);
    int n=0;
     
    if (read_nodes.good()){
        while (read_nodes >> n) {
            patches[cnt] = n;
            cnt++;
        }
    }
    else cout << "No info file found" << endl;
    read_nodes.close();
    
    
    // Loop over methods (D, L, C)
    for (unint method = 0; method < methods.size(); ++method)
    {
        // Loop over R0 values
        for (unint h = 0; h < r0.size(); ++h)
        {
            // Loop over epidemic seeds
            for (unint seed= 0; seed < patches.size(); ++seed)
            {
                
                int patch= patches[seed];
                int r_zero= r0[h];
                int inf_0=10;  // Initial number of infected individuals
                string  met=methods[method];
                
                int  t_a[n_patch];  // Arrival time for each patch
                
                unint  I_tot[n_patch];  // Total number of infections per patch
                memset(I_tot, 0, sizeof I_tot);
                
                int  t_p[n_patch];  // Time of peak infection
                double  tree[n_patch][n_patch];  // Invasion tree matrix
                memset(tree, 0, sizeof tree);
                
                
                if(r0[h]==1.5){r_zero=2;}
                
                string output_path;
                int len2;
                int len3;
                int len4;
                int t_star=0;   // Time when 5% of patches are infected
                int t_star2=0;
                int t_star3=0;
                
                vector<int> patches_inf;  // List of infected patches
                
                // Map method name to index
                int mmm=0;
                if(met=="Result_L1") { mmm=0;}
                if(met=="Result_C1") { mmm=1;}
                 
                // Create folder structure: Method/R0/seed
                vector<pair<string,int> > folders {make_pair("Metodo",mmm),make_pair("Ro",r_zero), make_pair("seed", patch)};

                
                // Loop over simulation runs
                for (int isim = 0; isim < Nsim; ++isim){
                    
                    double sum=0;
                    
                    memset(tree, 0, sizeof tree);
                    
                    // Repeat simulation until at least one transmission from seed occurs
                    while(sum==0){
                        
                        int len=0;
                        len2=0;
                        len3=0;
                        len4=0;
                        
                        // SEIR compartments for each patch at each time step
                        int  map_S[n_patch][TIME+1];  // Susceptible
                        memset(map_S, 0, sizeof map_S);
                        int  map_E[n_patch][TIME+1];  // Exposed
                        memset(map_E, 0, sizeof map_E);
                        int  map_I[n_patch][TIME+1];  // Infectious
                        memset(map_I, 0, sizeof map_I);
                        int  map_R[n_patch][TIME+1];  // Recovered
                        memset(map_R, 0, sizeof map_R);
                        
                        memset(tree, 0, sizeof tree);
                        
                        
                        patches_inf.resize(0);
                        patches_inf.resize(n_patch);
                        
                        
                        
                        memset(I_tot, 0, sizeof I_tot);
                        
                        memset(t_a, 0, sizeof t_a);
                        
                        memset(t_p, 0, sizeof t_p);
                        
                        unint   I_peak[n_patch];  // Peak number of infections
                        memset(I_peak, 0, sizeof  I_peak);
                        
                        
                        /* Memory allocation and variable definition */
                        
                        int  t0=0;  // Initial time
                        
                        
                        t_star=0;
                        
                        t_star2=0;
                        t_star3=0;
                        
                        
                        
                        double beta=(double)(r0[h]*mu);  // Transmission rate
                        int days=0;
                        
                        int month=8;  // Start in September (month 8, 0-indexed)
                        
                        int months=0;
                        int epidemic=1;
                        nodo D_map[n_patch];  // Node structure for each patch
                        
                        int  I_patch=0;      // Number of infected patches
                        int  I_patch_u=0;    // Number of infected urban patches
                        
                        // Main epidemic loop
                        while(epidemic!=0){
                            
                            /* Read population distribution and adjacency matrix */
                            // Set number of days per month
                            if(month==0 || month==2 || month==4|| month==6|| month==7|| month==9|| month==11){days=31;}
                            if(month==1){days=28; }
                            if(month==3||month==10||month==5||month==8){days=30;}
                            
                            // Read population and degree information for current month
                            string  network;
                            network="/data/giulia/aggregtaion_paper/metapop_file/site/degree_pop_nodi"+to_string(month)+"_"+met+".txt";
                            
                            /* degree includes self-loop */
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
                            
                            /* Initialize SEIR compartments */
                            
                            for(int i = 0; i <n_patch; ++i){
                                map_S[i][t0]=(int)D_map[i].n_subpop;
                                map_E[i][t0]=0;
                                map_I[i][t0]=0;
                                map_R[i][t0]=0;}
                            
                            // Initialize infection in seed patch
                            map_I[patch][t0]=inf_0;
                            map_S[patch][t0]= (int)(map_S[patch][t0]-map_I[patch][t0]);
                            
                            
                            
                            // Read coupling matrix for current month
                            string  matrix;
                            vector<vector<int>> contact_list_in; // Incoming adjacency lists
                            vector<vector<double>> prob_list_in; // Incoming coupling probabilities
                            contact_list_in.resize(0);
                            contact_list_in.resize(n_patch+1);
                            prob_list_in.resize(0);
                            prob_list_in.resize(n_patch+1);
                            
                            vector<vector<int>> contact_list_out; // Outgoing adjacency lists
                            vector<vector<double>> prob_list_out; // Outgoing coupling probabilities
                            contact_list_out.resize(0);
                            contact_list_out.resize(n_patch+1);
                            prob_list_out.resize(0);
                            prob_list_out.resize(n_patch+1);
                            double prob_stay[n_patch];  // Probability of staying in home patch
                            memset(prob_stay, 0, sizeof prob_stay);
                            matrix="/data/giulia/aggregtaion_paper/metapop_file/site/"+met+"_"+ to_string(month) +"_metapop.txt";
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
                            
                            // Loop over days in current month
                            for(int day_of_month=0;day_of_month<days;++day_of_month){
                                day=day_of_month+months;
                                int scenarios_tot=0;
                                
                                
                                // Loop over all patches
                                for(int i = 0;i<n_patch; ++i){
                                    
                                    int new_time = (day + 1);
                                    const unint susc = map_S[i][day];
                                    const unint exposed = map_E[i][day];
                                    const unint infect = map_I[i][day];
                                    const unint recovered = map_R[i][day];
                                    const int N_sub = susc + exposed+ infect + recovered;
                                    float lambda_stay=0;  // Force of infection from staying
                                    
                                    float  lambda_v[D_map[i].degree_in-1];  // Force of infection from visitors
                                    memset(lambda_v, 0, sizeof lambda_v);
                                    float  lambda_r[D_map[i].degree_out-1];  // Force of infection from returning residents
                                    memset(lambda_r, 0, sizeof lambda_r);
                                    
                                    int    N_sub_j=0;
                                    float  N_sub_eff=0;      // Effective population in patch i
                                    float  N_sub_eff_j=0;    // Effective population in patch j
                                    float  I_sub_eff_j=0;    // Effective infectious in patch j
                                    float  I_sub_eff=0;      // Effective infectious in patch i
                                    
                                    I_tot[i]+=infect;
                                    
                                    
                                    // Calculate effective population accounting for mobility
                                    N_sub_eff = prob_stay[i]*N_sub;
                                    I_sub_eff = prob_stay[i]*infect;
                                    
                                    // Add visitors from other patches
                                    for(int j=0;j<D_map[i].degree_in-1;++j){
                                        int node= contact_list_in[i][j];
                                        if(node!=i){
                                            
                                            N_sub_eff += (prob_list_in[i][j]*D_map[node].n_subpop);
                                            I_sub_eff += (prob_list_in[i][j]*map_I[node][day]);
                                        } }
                                    
                                    // Calculate force of infection from visitors
                                    for(int j=0;j<D_map[i].degree_in-1;++j){
                                        int node= contact_list_in[i][j];
                                        if(node!=i){
                                            lambda_v[j]=(double)((prob_stay[i]*prob_list_in[i][j]*map_I[node][day])/N_sub_eff);
                                        } }
                                    
                                    int test=0;
                                    
                                    // Calculate force of infection from returning residents
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
                                    
                                    
                                    /* Reaction processes (SEIR transitions) */
                                    
                                    int new_infected =  0;
                                    int new_exposed =  0;
                                    int new_recovered = 0;
                                    
                                    scenarios_tot=D_map[i].degree_in+D_map[i].degree_out;
                                    
                                    unint dif[scenarios_tot];  // Multinomial outcomes
                                    double diffusion_rates[scenarios_tot];  // Infection rates from each source
                                    
                                    memset(dif, 0, sizeof dif);
                                    memset(diffusion_rates, 0, sizeof diffusion_rates);
                                    
                                    
                                    int count=1;
                                    double f=0;
                                    
                                    // Build diffusion rates array for multinomial draw
                                    // Rate from visitors
                                    for(int k=1;k<D_map[i].degree_in;++k){
                                        diffusion_rates[count]=beta*lambda_v[k-1];
                                        f+=diffusion_rates[count];
                                        count+=1;
                                    }
                                    
                                    // Rate from returning residents
                                    for(int k=D_map[i].degree_in;k<scenarios_tot-1;++k){
                                        diffusion_rates[count]=(beta*lambda_r[k-D_map[i].degree_in]);
                                        f+=diffusion_rates[count];
                                        count+=1;
                                    }
                                    
                                    // Rate from staying
                                    diffusion_rates[scenarios_tot-1]=(beta*lambda_stay);
                                    f+=diffusion_rates[scenarios_tot-1];
                                    
                                    // Probability of no infection
                                    diffusion_rates[0]=(1-f);
                                    
                                    
                                    // Draw new exposures from multinomial distribution
                                    if(susc!=0){
                                        gsl_ran_multinomial(r,scenarios_tot,susc,diffusion_rates,dif);
                                        
                                        
                                        for(int d=1;d<scenarios_tot;++d){
                                            new_exposed+=dif[d]; }
                                    }
                                    
                                    
                                    // E -> I transition
                                    if(exposed!=0){
                                        new_infected=(int)gsl_ran_binomial(r,epsilon, exposed);
                                    }
                                    
                                    // I -> R transition
                                    if(infect!=0){
                                        new_recovered=(int)gsl_ran_binomial(r, mu, infect);
                                    }
                                    
                                    // Update SEIR compartments
                                    map_S[i][new_time] = (susc- new_exposed);
                                    map_E[i][new_time] = (exposed+new_exposed)-new_infected;
                                    map_I[i][new_time] = (infect+new_infected)-new_recovered;
                                    map_R[i][new_time] = (recovered+new_recovered);
                                    
                                    double temp_var=0;
                                    
                                    // Track invasion tree: first time patch becomes exposed
                                    if((map_E[i][new_time]!=0)&(map_S[i][day]==D_map[i].n_subpop)){
                                        t_a[i]=new_time;  // Record arrival time
                                        
                                        if(i<6){I_patch_u+=1;}  // Count urban patches (first 6 are urban)
                                        patches_inf[len]=i;
                                        
                                        len+=1;
                                        I_patch+=1;
                                        
                                        // Track time when urban areas reached from rural seed
                                        if(patch>=6){
                                            if(i<6){
                                                if(t_star2==0){
                                                    t_star2=new_time;
                                                } }  }
                                        
                                        // Count number of sources of infection
                                        for(int l=1;l<scenarios_tot-1;++l){
                                            if(dif[l]!=0){
                                                temp_var+=1;
                                            }}
                                        
                                        // Build invasion tree: assign edges based on infection sources
                                        for(int l=1;l<scenarios_tot-1;++l){
                                            if(dif[l]!=0){
                                                
                                                // Infection from visitor
                                                if(l<D_map[i].degree_in){
                                                    int node= contact_list_in[i][l-1];
                                                    tree[node][i]=(1./temp_var);}
                                                // Infection from returning resident
                                                if(l>=D_map[i].degree_in) {
                                                    int node= contact_list_out[i][l-D_map[i].degree_in];
                                                    tree[node][i]=(1./temp_var);
                                                }
                                            } }
                                    }
                                    
                                    // Track peak infection
                                    if(I_tot[i]>I_peak[i]){
                                        I_peak[i]=I_tot[i];
                                        t_p[i]=new_time;}
                                    
                                }  // End loop over patches
                                
                                // Track time when 5% of patches infected (35 out of ~700)
                                if((I_patch>34) & (len2==0)){
                                    if(t_star==0){
                                        t_star=day+1;
                                        len2=len;}}
                                
                                
                                // Track time when all 6 urban patches infected
                                if((I_patch_u==6) & (t_star3==0)){
                                    t_star3=day+1;
                                }
                                
                                // Check if epidemic is extinct
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
                            }/* End day loop */
                            
                            months+=days;
                            month+=1;
                            if(month==12){month=0;}
                            
                        }/* End epidemic loop */
                        
                        // Check if at least one transmission occurred from seed
                        for(int j=0;j<n_patch;++j){
                            if((j!=patch) & (tree[patch][j]!=0)){
                                sum+=1;
                            } }
                    }/* End while(sum==0) */
                    
                    
                    // Create output directory
                    output_path = path_creator(basic_path, folders);
                    
                    // Write arrival times to file
                    ofstream myfileb;
                     
                    myfileb.open(output_path +"/offset_"+to_string(offset)+"_"+ to_string(isim) + "_arrival.txt");
                    if (myfileb.is_open()){
                        for(int j=0;j<n_patch;++j){
                            myfileb <<j<<" "<< t_a[j]<<"\n";
                        }
                        myfileb.close();
                    }  else cout <<"Unable to open file";
                     
                     
                     
                 
                    // Write list of infected patches to file
                    ofstream myfilea;
                    myfilea.open(output_path + "/offset_"+to_string(offset) + "_run" +to_string(isim) + "patches_inf.txt");
                    if (myfilea.is_open()){
                        for(int j=0;j<n_patch;++j){
                            myfilea <<patches_inf[j]<<"\n";
                        }
                        myfilea.close();
                    }  else cout <<"Unable to open file";
                       
                    /* 
                    // Optionally write invasion tree to file (commented out)
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

                     
                }/* End simulation runs loop */
                    
              
                
            }// End R0 loop
        } // End seed loop
    } // End method loop
    
    
    
}/* End main */
