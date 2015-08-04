#include <iostream>
#include <fstream>
#include "dart/common/Console.h"
#include "dart/common/Uri.h"
#include "LocalResourceRetriever.h"
#include "LocalResource.h"

namespace dart {
namespace common {

bool LocalResourceRetriever::exists(const std::string& _uri)
{
  common::Uri uri;
  if(!uri.fromString(_uri))
  {
    dtwarn << "[LocalResourceRetriever::exists] Failed parsing URI '"
           << _uri << "'.\n";
    return false;
  }

  // Open and close the file to check if it exists. It would be more efficient
  // to stat() it, but that is not portable.
  if(uri.mScheme.get_value_or("file") != "file")
    return false;
  else if (!uri.mPath)
    return false;

  return std::ifstream(*uri.mPath, std::ios::binary).good();
}

common::ResourcePtr LocalResourceRetriever::retrieve(const std::string& _uri)
{
  common::Uri uri;
  if(!uri.fromString(_uri))
  {
    dtwarn << "[LocalResourceRetriever::retrieve] Failed parsing URI '"
           << _uri << "'.\n";
    return nullptr;
  }

  if(uri.mScheme.get_value_or("file") != "file")
    return nullptr;
  else if (!uri.mPath)
    return nullptr;

  const auto resource = std::make_shared<LocalResource>(*uri.mPath);
  if(resource->isGood())
    return resource;
  else
    return nullptr;
}

} // namespace common
} // namespace dart
