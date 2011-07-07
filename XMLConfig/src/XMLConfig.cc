#include "XMLConfig.h"

#include "tinyxml/tinyxml.h"

using namespace std;

XMLConfig::XMLConfig()
{
  document = 0;
  root_node = 0;

  return;
}

XMLConfig::XMLConfig(TiXmlNode* root_node_)
{
  root_node = root_node_;
  document = 0;
}

XMLConfig::~XMLConfig()
{
  if (document != 0)
    delete document;

  return;
}

int XMLConfig::Load(const string& filename)
{ 
  document = new TiXmlDocument(filename.c_str());
  bool loadOkay = document->LoadFile();

  if (!loadOkay)
    {
      printf("Failed to load file \"%s\"\n", filename.c_str());
      return -1;
    }

  root_node = document->FirstChild();

  return 0;
}

void XMLConfig::Print(TiXmlNode* node)
{
  if (!node)
    return;

  TiXmlNode* parent = node;
  TiXmlNode* child;
  TiXmlText* text;
  int t = parent->Type();
  
  switch (t)
    {
    case TiXmlNode::DOCUMENT:
      printf("Document");
      break;
      
    case TiXmlNode::ELEMENT:
      printf("Element [%s]", parent->Value());
      break;
      
    case TiXmlNode::COMMENT:
      printf("Comment: [%s]", parent->Value());
      break;
      
    case TiXmlNode::UNKNOWN:
      printf("Unknown");
      break;
      
    case TiXmlNode::TEXT:
      text = parent->ToText();
      printf("Text: [%s]", text->Value());
      break;
      
    case TiXmlNode::DECLARATION:
      printf("Declaration");
      break;
    default:
      break;
    }
  printf( "\n" );
  for (child = parent->FirstChild(); child != 0; child = child->NextSibling()) 
    Print(child);
  
  return;
}

XMLConfig *XMLConfig::GetChildrenAsRoot(const string& parent_path, int index)
{
  TiXmlNode *parent = FollowPath(parent_path);
  
  if(parent == 0)
    return NULL;

  TiXmlNode *child = parent->FirstChild();

  for(int i=0; i < index; ++i)  
    {
      child = child->NextSibling();
      if(child == NULL)
        return NULL;
    }

  return new XMLConfig(child);
}

XMLConfig *XMLConfig::GetElementAsRoot(const string& path)
{
  TiXmlNode *node = FollowPath(path);
  
  if (node == 0)
    return NULL;

  return new XMLConfig(node);
}

TiXmlNode* XMLConfig::FollowPath(const string& path)
{
  char tmp_path[strlen(path.c_str())];
  strcpy(tmp_path, path.c_str());
 
  char* part = strtok(tmp_path, "/");

  int found_it = 0;
  int tokens = 0;

  TiXmlNode* node;

  if (document != 0)
    node = document;
  else if (root_node != 0)
    node = root_node;
  else
    return 0;

  while (part != NULL)
  {
    tokens++;
    for (TiXmlNode* child = node->FirstChild(); 
         child != 0; child = child->NextSibling())
      {
        if (strcmp(part, child->Value()) == 0)
          {
            node = child;
            found_it++;
            break;
          }
      }

    part = strtok (NULL, "/");
  }

  if (tokens == found_it)
    return node;

  return 0;
}

bool XMLConfig::HasElement(const string& path)
{
  if (FollowPath(path) != 0)
    return true;

  return false;
}

int XMLConfig::GetInt(const string& path, int default_value)
{
  TiXmlNode* t = FollowPath(path);

  if (t == 0)
    return default_value;

  assert(t->FirstChild()->Type() == TiXmlNode::TEXT);
  return atoi(t->FirstChild()->ToText()->Value());
}

float XMLConfig::GetFloat(const string& path, float default_value)
{
  TiXmlNode* t = FollowPath(path);

  if (t == 0)
    return default_value;

  assert(t->FirstChild()->Type() == TiXmlNode::TEXT);

  return atof(t->FirstChild()->ToText()->Value());
}

void XMLConfig::GetString(const string& path, string& str)
{
  TiXmlNode* t = FollowPath(path);
  
  if (t == 0)
    {
      printf("Unable to find %s\n", path.c_str());
      assert(t != 0);
    }

  assert(t->FirstChild()->Type() == TiXmlNode::TEXT);
  
  str = string(t->FirstChild()->ToText()->Value());
  
  return;
}

int XMLConfig::GetTupleCount(const string& path)
{
  TiXmlNode* t = FollowPath(path);

  if (t == 0)
    {
      printf("Unable to find %s\n", path.c_str());
      assert(t != 0);
    }

  assert(t->FirstChild()->Type() == TiXmlNode::TEXT);

  char buf[strlen(t->FirstChild()->ToText()->Value())];
  strcpy(buf, t->FirstChild()->ToText()->Value());

  char* part = strtok(buf, " ");

  int count = 0;

  while (part != NULL)
    {
      part = strtok (NULL, " ");
      count++;
    }

  return count;
}

int XMLConfig::GetTupleInt(const string& path, int index, int default_value)
{
  TiXmlNode* t = FollowPath(path);

  if (t == 0)
    return default_value;

  assert(t->FirstChild()->Type() == TiXmlNode::TEXT);

  char buf[strlen(t->FirstChild()->ToText()->Value())];
  strcpy(buf, t->FirstChild()->ToText()->Value());

  char* part = strtok(buf, " ");

  int count = 0;

  while (part != NULL)
    {
      if (count == index) 
        return atoi(part);
      part = strtok (NULL, " ");
      count++;
    }

  assert(count < index);

  return 0;
}

float XMLConfig::GetTupleFloat(const string& path, int index, 
                               float default_value)
{
  TiXmlNode* t = FollowPath(path);

  if (t == 0)
    return default_value;

  assert(t->FirstChild()->Type() == TiXmlNode::TEXT);

  char buf[strlen(t->FirstChild()->ToText()->Value())];
  strcpy(buf, t->FirstChild()->ToText()->Value());

  char* part = strtok(buf, " ");

  int count = 0;

  while (part != NULL)
    {
      if (count == index) 
        return atof(part);
      part = strtok (NULL, " ");
      count++;
    }

  assert(count < index);

  return 0;
}

bool XMLConfig::HasAttribute(const std::string& attribute)
{
  const char* ret;
  if ((ret = root_node->ToElement()->Attribute(attribute.c_str())) == NULL)
    return false; 

  return true;
}
 
void XMLConfig::GetAttributeString(const std::string& attribute, 
                                   std::string& str)
{
  const char* ret;
  if ((ret = root_node->ToElement()->Attribute(attribute.c_str())) == NULL)
    str = "";
  else
    str = string(ret);

  return;
}
 
float XMLConfig::GetAttributeFloat(const std::string& attribute, 
                                   float default_value)
{
  const char* ret;
  if ((ret = root_node->ToElement()->Attribute(attribute.c_str())) == NULL)
    return default_value;
  
  return atof(ret);
}
 
float XMLConfig::GetAttributeTupleFloat(const std::string& attribute, 
                                        int index, float default_value)
{
  const char* ret;
  if ((ret = root_node->ToElement()->Attribute(attribute.c_str())) == NULL)
    return default_value;

  char buf[strlen(ret)];
  strcpy(buf, ret);

  char* part = strtok(buf, " ");

  int count = 0;

  while (part != NULL)
    {
      if (count == index) 
        return atof(part);
      part = strtok (NULL, " ");
      count++;
    }

  assert(count < index);

  return 0;
}
