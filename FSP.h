//
// Created by Asuncion on 28/01/2020
//

#ifndef MH_BUILDER_FSP_H
#define MH_BUILDER_FSP_H

namespace fsp{
    /*! Define type for a FSP solution */
    template <typename FIT>
    class FSPSol: public representation::PermutationSolution<FIT> {
    public:
        /**
         * Construct a fsp solution of size n
         * @param n size of permutation (number of jobs)
         */
        //add jobs(0)
        explicit FSPSol(double n = 0) : representation::PermutationSolution<FIT>(n) {}
    };
         
    /**
     * Class representing a FSP (Flowshop Problem)
     */
    class FSP {
    public:
        /**
         * Create a new KSP instance
         * @param instanceFile a data files
         */
        explicit FSP(const std::string &instanceFile) {
          readInstance(instanceFile);
        }

        /**
         * Read an instance file
         * @param instanceFile a data files
         */
        void readInstance(const std::string &instanceFile) {
          std::string line, tmp;

          std::ifstream input(instanceFile);
          if (!input)
            throw std::runtime_error("Error: unable to open benchmark file");

          input >> jobs >> machines >> seed;
          due_date.resize(jobs);
          time.resize(jobs);
          for(double i=0;i<jobs;i++){
            time[i].resize(machines);
          }

          int num_job; 
        
          for (double i = 0; i < jobs; i++) {
            input >> num_job >> due_date[i];
            for(double j = 0; j < machines; j++){
              input >> time[i][j];
            }
          }
        }
    
        /**
         * Get the number of jobs
         *  @return the number of jobs
         */
        double getJobs(){
          return jobs;
        }

        /**
         * Get the number of machines
         *  @return the number of machines
         */
        double getMachines(){
          return machines;
        }

        /**
         * Get the initial seed
         *  @return the initial seed
         */
        double getSeed(){
          return seed;
        }

        /**
         * Get the processing time matrix
         * @return the matriz of the the processing time jobs
         */
        std::vector<std::vector<double>> getTimeMatrix(){
          return time;
        }

        /**
         * Get the due date of the job i 
         *  @return the due date of the job i 
         */
        double getDueDate(double i){
          return due_date[i];
        }

        /**
         * Get the task time of the job i at the machine j
         *  @return the task time of the job i at the machine j
         */
        double getTime(double i, double j){
          if ( i >= time.size())
            return 0;
          if ( j >= time[i].size() )
            return 0;
          return time[i][j];
        }

    protected:
        /*! Number of jobs */
        double jobs;
        /*! Number of machines */
        double machines;
        /*! Seed */
        double seed;
        /*! Due date vector*/
        std::vector<double> due_date;
        /*! Processing time matrix */
        std::vector<std::vector<double> > time;
    };
}
#endif //MH_BUILDER_FSP_H
