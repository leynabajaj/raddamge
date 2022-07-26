#!/apps/python/PRO/bin/python
from subprocess import call
import sys,os,time

def main():

    ### always check that you have the ranges you need
    beamEnergy=[200,300]
    #beamEnergy=[10]
    #beamEnergy=[500, 1000, 1500, 2000, 3000, 5000, 8000]
    #beamEnergy=[500, 600, 700, 800, 900, 1000, 1250, 1500, 1750, 2000, 2250, 2500, 2750, 3000]

    # material="G4_W"
    # thickness=[1,2,5,10,15]

    material="G4_Pb"
    thickness=[11,12,13,14,16]

    #material="G4_Al"
    #thickness=[5,10,20,30,40,50,60]
    #thickness=[1,2,3,4,5,6,7]

    #material="G4_STAINLESS-STEEL"
    #thickness=[1,2,5,10,15,20]

    # material="G4_Fe"
    # thickness=[2.44,12.20,24.39,36.59]

    #material="Mat_Boron"
    #thickness=[3]

    # material="G4_POLYETHYLENE"
    # thickness=[20.41,102.04,204.08,306.12]

    #material="Borated_Polythene"
    #thickness=[5,20,40,60,80,100,150,200]

    ### logistic info
    sourceDir="/w/halla-scshelf2102/moller12gev/chandan/sim/radDamage"

    for thick in thickness: #tgt length
        for beamE in beamEnergy: # E of the beam
            jobName="Mu%04dMeV-%03.1fcm"%(beamE,thick) + material

            if not os.path.exists(sourceDir+"/scripts/"+jobName):
                os.makedirs(sourceDir+"/scripts/"+jobName)
            
            fmac=open(sourceDir+"/scripts/"+jobName+"/run.mac","w")
            fmac.write("/msc/event/setPrintModulo 10000\n")
            fmac.write("/msc/gun/setGunEnergy "+str(beamE)+" MeV\n")
            fmac.write("/msc/det/setTargetMaterial "+material+"\n")
            fmac.write("/msc/det/setTargetLength "+str(thick)+" cm\n")
            fmac.write("/msc/det/updateGeometry\n")
            fmac.write("/msc/step/setOutFileName E_TranRef"+jobName+"-1e5.root\n")
            fmac.write("/run/beamOn 100000\n")
            fmac.close()

            os.chdir(sourceDir+"/scripts/"+jobName)
            call([sourceDir+"/build/radTest","-m",sourceDir+"/scripts/"+jobName+"/run.mac"])
        
    print "All done"  
    
if __name__ == '__main__':
    main()
                  
