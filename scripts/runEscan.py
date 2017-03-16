#!/apps/python/PRO/bin/python
from subprocess import call
import sys,os,time

def main():

    ### always check that you have the ranges you need
    beamEnergy=[25, 50, 100, 150, 200, 300, 400]

    # material="G4_W"
    # thickness=[1,5,10,15]

    # material="G4_Pb"
    # thickness=[1.69,8.47,16.95,25.42]

    # material="G4_Fe"
    # thickness=[2.44,12.20,24.39,36.59]

    material="G4_CONCRETE"
    thickness=[8.33,41.66,83.33,125.00]

    # material="G4_POLYETHYLENE"
    # thickness=[20.41,102.04,204.08,306.12]

    ### logistic info
    sourceDir="/home/ciprian/prex/radDamage"

    for thick in thickness: #tgt length
        for beamE in beamEnergy: # E of the beam
            jobName="%03dMeV-%06.2fcm"%(beamE,thick) + material

            if not os.path.exists(sourceDir+"/scripts/"+jobName):
                os.makedirs(sourceDir+"/scripts/"+jobName)
            
            fmac=open(sourceDir+"/scripts/"+jobName+"/run.mac","w")
            fmac.write("/msc/event/setPrintModulo 10000\n")
            fmac.write("/msc/gun/setGunEnergy "+str(beamE)+" MeV\n")
            fmac.write("/msc/det/setTargetMaterial "+material+"\n")
            fmac.write("/msc/det/setTargetLength "+str(thick)+" cm\n")
            fmac.write("/msc/det/updateGeometry\n")
            fmac.write("/msc/step/setOutFileName o-tranRef-"+jobName+"-1e5.root\n")
            fmac.write("/run/beamOn 100000\n")
            fmac.close()

            os.chdir(sourceDir+"/scripts/"+jobName)
            call([sourceDir+"/build/radTest","-m",sourceDir+"/scripts/"+jobName+"/run.mac"])
        
    print "All done"  
    
if __name__ == '__main__':
    main()
                  
