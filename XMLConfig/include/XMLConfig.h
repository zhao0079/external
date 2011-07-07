#ifndef __XMLCONFIG__
#define __XMLCONFIG__
#include <string>

class TiXmlNode;
class TiXmlDocument;
class XMLConfig
{
 public:
  XMLConfig();
  XMLConfig(TiXmlNode* root_node);
  ~XMLConfig();

  int Load(const std::string& filename);
  void Print() { Print(root_node); }
  bool HasElement(const std::string& path);
  int GetInt(const std::string& path, int default_value);
  int GetTupleInt(const std::string& path, int index, int default_value);
  float GetFloat(const std::string& path, float default_value);
  float GetTupleFloat(const std::string& path, int index, float default_value);
  int GetTupleCount(const std::string& path);
  void GetString(const std::string& path, std::string& str);

  bool HasAttribute(const std::string& attribute);
  void GetAttributeString(const std::string& attribute, 
                          std::string& str);
  float GetAttributeFloat(const std::string& attribute, float default_value);
  float GetAttributeTupleFloat(const std::string& attribute, 
                               int index, float default_value);
  
  XMLConfig* GetChildrenAsRoot(const std::string& path, int index);
  XMLConfig* GetElementAsRoot(const std::string& path);
  bool IsValid() { return ( (document != 0) || (root_node != 0) ); }
  
 private:
  void Print(TiXmlNode* node);
  TiXmlNode* FollowPath(const std::string& path);
  
  TiXmlDocument* document;
  TiXmlNode* root_node;
};
#endif
