//
// Created by Asuncion on 03/02/2020
//

#ifndef MH_BUILDER_INITPERMU_H
#define MH_BUILDER_INITPERMU_H

//algo d'inisialisation 
namespace fsp {
    /**
     * Class representing an initial solution
     * @tparam PERMU initial solution
     */
    template <typename PERMU>
    class InitPermu{
    public:

        /**
         * Constructor of an fsp initial solution 
         * @param _instance instance of the flowshop problem
         * @param _eval
         */
        explicit InitPermu(FSP &_instance) : instance(_instance){}

        //NEH algorithm 
        void operator()(PERMU &permu){
            /*! number of jobs*/
            double J = instance.getJobs(); 
            /*! number of machines*/
            double M= instance.getMachines();
            /*! processing time vector*/
            std::vector<double> tpo_ini_1;
            /*! processing time vector in descending order*/
            std::vector<double> tpo_ini_2;
            for(double i=0;i<J;i++){
                double proctime=0;
                for(double j=0;j<M;j++){          
                    proctime+=instance.getTimeMatrix()[i][j];
                }
                tpo_ini_1.push_back(proctime);
            }
            tpo_ini_2=tpo_ini_1;
            std::sort(tpo_ini_2.begin(), tpo_ini_2.end(), std::greater <double>());
            /*! initial solution empty */
            PERMU sol_ini;
            for(double i=0;i<J;i++){
                double index=-1;
                //searching the index of the order vector
                std::vector<double>::iterator it=std::find(tpo_ini_1.begin(),tpo_ini_1.end(),tpo_ini_2[i]);
                index = std::distance(tpo_ini_1.begin(), it);
                tpo_ini_1[index]=-1;
                sol_ini.push_back(index);
            }
            //insert each job to another place if it minimize the date of the last task 
            //NEH ...
            for(double n=1;n<J;n++){
                //sol_temp = sol_ini[0:n]
                PERMU sol_temp;
                for(double k=0;k<=n;k++){
                    sol_temp.push_back(sol_ini[k]);
                }
                //std::cout<<"sol temp "<< sol_temp<<"\n";
                for(double i=0;i<=n;i++){
                    PERMU sol_alt;
                    sol_alt=insertion(sol_temp,n,i);
                    //std::cout<<"sol alt "<< sol_alt<<"\n";
                    //if it minimize the date of the last task 
                    if(time(sol_alt,'M')<time(sol_temp,'M')){
                        //sol_ini[0:n]=sol_temp
                        for(double k =0;k<=n;k++){
                            sol_ini.erase(sol_ini.begin() + k);
                            sol_ini.insert(sol_ini.begin() + k, sol_alt[k]);
                        }
                        break;
                    }
                }
            }
            permu = sol_ini;
        }
        
        /**
         * to move a job from one place to another by inserting it to another place 
         * @param S solution vector (permutation)
         * @param i the index of the current place job
         * @param j the index of the new place job
         * @return the solution after the job change 
         */
        PERMU insertion(PERMU S, double i, double j) {
            double element = S[i];
            PERMU A (S.size());
            A=S;
            typename PERMU::iterator it;
            it =A.begin();
            //delete the i^th element
            A.erase(it+i);
            //insert the element in the j^th position
            A.insert(it+j, element);
            return A;
        } 

        /** 
         * Compute the makespan or the flowtime for a solution
         * @param sol the solution
         * @param option M or F for makespan or flowtime
         * @return the makespan or the flowtime of the solution
         */
        double time(PERMU sol, char option){
            /*! number of jobs */
            //double N= instance.getJobs();
            double N= sol.size();
            /*! number of machines */
            double M= instance.getMachines();
            /*! processing time matrix for each job and machine */
            std::vector<std::vector<double > >date (N, std::vector<double> (M,0)); 
            /*! computing the first row and column */
            date[0][0]=instance.getTimeMatrix()[sol[0]][0];
            for(double i=1;i<N;i++){
                date[i][0]= instance.getTimeMatrix()[sol[i]][0] + date[i-1][0];
            }
            for(double i=1;i<M;i++){
                date[0][i]=instance.getTimeMatrix()[sol[0]][i] + date[0][i-1];
            }
            /* dynamic programation */
            for(double i=1;i<N;i++){
                for(double j=1;j<M;j++){
                    date[i][j]= std::max(date[i-1][j], date[i][j-1]) + instance.getTimeMatrix()[sol[i]][j];
                }
            }
            /*! makespan */
            if(option=='M'){
                return date[N-1][M-1];
            } 
            /*! flowtime */
            else if(option=='F'){
                double flowtime = 0;
                for(double i =0;i<N;i++){
                    flowtime+=date[i][M-1];
                }
                return flowtime;
            } 
            else{
                return 0;
            }
            
        }
    
    protected:
        FSP &instance; 
    };
}
#endif //MH_BUILDER_INITPERMU_H