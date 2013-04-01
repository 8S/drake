
#include <fstream>
#include <iostream>
#include "../urdf.h"

using namespace std;

int main(int argc, char* argv[])
{
  // todo: pull urdf filename off the command line
  std::string filename("../../../examples/Atlas/urdf/atlas_minimal_contact.urdf");
  std::string xml_string;
  std::fstream xml_file(filename.c_str(), std::fstream::in);
  if (xml_file.is_open())
  {
    while ( xml_file.good() )
    {
      std::string line;
      std::getline( xml_file, line);
      xml_string += (line + "\n");
    }
    xml_file.close();
  }
  else
  {
    std::cerr << "Could not open file ["<<filename.c_str()<<"] for parsing."<< std::endl;
    return -1;
  }
  
  // parse URDF to get model
  RigidBodyManipulator* model = parseURDFModel(xml_string);
  
  // run kinematics with second derivatives 100 times
  double q[34];
  memset(q,0,sizeof(double)*34);
  
  model->doKinematics(q,true);

  Vector4d zero;
  zero << 0,0,0,1; 
  
  for (int i=0; i<34; i++) 
    cout << "forward kin: " << model->bodies[i+1].linkname << " is at " << model->forwardKin(i+1,zero,true).transpose() << endl;
 
  delete model;
  return 0;
}