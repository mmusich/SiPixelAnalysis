// -*- C++ -*-
//
// Package:    SiPixelAnalysis/ShallowPixelAnalyzer
// Class:      ShallowPixelAnalyzer
//
/**\class ShallowPixelAnalyzer ShallowPixelAnalyzer.cc SiPixelAnalysis/ShallowPixelAnalyzer/plugins/ShallowPixelAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Marco Musich
//         Created:  Tue, 04 Sep 2018 07:48:44 GMT
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
#include "TProfile.h"

//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.


using reco::TrackCollection;

class ShallowPixelAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>  {
   public:
      explicit ShallowPixelAnalyzer(const edm::ParameterSet&);
      ~ShallowPixelAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------
      edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster> > pixelSrcToken_;
  
      int maxLS,maxBx;

      edm::Service<TFileService> fs;
      
      TProfile* p_nClust_per_bx;        
      TProfile* p_nClust_per_LS;
      TProfile* p_nClust_vs_lumi;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
ShallowPixelAnalyzer::ShallowPixelAnalyzer(const edm::ParameterSet& iConfig)
{
  pixelSrcToken_ = consumes<edmNew::DetSetVector<SiPixelCluster>>(iConfig.getParameter<edm::InputTag>("pixelSrc"));
  //now do what ever initialization is needed
  maxLS=-999; 
  maxBx=-999;
}


ShallowPixelAnalyzer::~ShallowPixelAnalyzer()
{

   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called for each event  ------------
void
ShallowPixelAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   int bx = iEvent.bunchCrossing();
   int ls = iEvent.id().luminosityBlock();

   edm::Handle<edmNew::DetSetVector<SiPixelCluster>> inputPixel;
   iEvent.getByToken(pixelSrcToken_, inputPixel);
   if (!inputPixel.isValid()) return;

   int count=0;
   edmNew::DetSetVector<SiPixelCluster>::const_iterator it;
   for (it = inputPixel->begin(); it != inputPixel->end(); ++it) {
     //auto id = DetId(it->detId());
     count++;
   }
   
   p_nClust_per_bx->Fill(bx,count); 
   p_nClust_per_LS->Fill(ls,count); 

}


// ------------ method called once each job just before starting event loop  ------------
void
ShallowPixelAnalyzer::beginJob()
{
  p_nClust_per_bx = fs->make<TProfile>("nPixelClustersVsBx","n. of Pixel clusters vs bx;bx id;#LT n.clusters/event #GT",3500,-0.5,3495.);     
  p_nClust_per_LS = fs->make<TProfile>("nPixelClustersVsLS","n. of Pixel clusters vs LS;LS; #LT n.clusters/event #GT",100,0,100); 
}

// ------------ method called once each job just after ending the event loop  ------------
void
ShallowPixelAnalyzer::endJob()
{
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void
ShallowPixelAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(ShallowPixelAnalyzer);
