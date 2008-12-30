#ifndef sample_0090scenesample_sceneadmin_h
#define sample_0090scenesample_sceneadmin_h

#include"../../source/config/define.h"

#include"../../source/framework/isceneadmin.h"



class SceneAdmin : public Maid::ISceneAdmin
{
public:

protected:
  virtual Maid::SPSCENE CreateFirstScene(); 
  virtual Maid::SPSCENE CreateNextScene( const Maid::SPSCENEOUTPUT& pOut );
};

#endif



