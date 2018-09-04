import FWCore.ParameterSet.Config as cms

process = cms.Process("Demo")

process.load("FWCore.MessageService.MessageLogger_cfi")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1))

process.source = cms.Source("PoolSource",
    # replace 'myfile.root' with the source file you want to use
                            fileNames = cms.untracked.vstring('file:/tmp/musich/file.root',
                                                              'file:0C9A33AA-5B86-E811-9A0E-FA163E60AAEB.root',
                                                              'file:90333728-7186-E811-B10C-FA163E9A7438.root',
                                                              'file:B8DAA0F0-5B86-E811-A110-FA163E658888.root')
                            )

###################################################################
# The module
###################################################################
process.demo = cms.EDAnalyzer('ShallowPixelAnalyzer',
                              pixelSrc = cms.InputTag("siPixelClusters")
                              )

###################################################################
# Output name
###################################################################
process.TFileService = cms.Service("TFileService",
                                   fileName = cms.string("test.root")
                                   )


process.p = cms.Path(process.demo)
