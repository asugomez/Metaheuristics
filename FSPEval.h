//
// Created by Asuncion on 13/02/2020.
//

#ifndef MH_BUILDER_FSPEVAL_H
#define MH_BUILDER_FSPEVAL_H

namespace fsp {
  /**
   * Class representing an evaluator for Flowshop problem
   */
  template <typename FSPSol>
  class FSPEval : public core::Eval<FSPSol> {
  public:
      
    /**
     * Constructor of the evaluation
     * @param _fsp data for evaluator
     */
    explicit FSPEval(FSP &_fsp) : fsp(_fsp) {}

    /**
     * Evaluate a FSP solution
     * @param sol a solution
     */
    void operator()(FSPSol &sol) override {
      /*! fitness solution */
      auto &fit = sol.fitness();
      auto objv = fit.objectives();

      objv.resize(1);
      /*! makespan */
      objv[0]=(fsp::InitPermu<FSPSol>(fsp).time(sol,'M'));
      /*! flowtime */
      objv.push_back(fsp::InitPermu<FSPSol>(fsp).time(sol,'F'));

      fit.objectives(objv);
    }

  protected:
      /*! FSP data */
      FSP fsp; 
  };    
  
}

#endif //MH_BUILDER_FSPEVAL_H