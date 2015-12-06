

std::vector<std::string> &
split (const std::string &s, char delim, std::vector<std::string> &elems)
{
  std::stringstream ss (s);
  std::string item;
  while (std::getline (ss, item, delim))
  {
    elems.push_back (item);
  }
  return elems;
}

std::vector<std::string>
split (const std::string &s, char delim)
{
  std::vector<std::string> elems;
  split (s, delim, elems);
  return elems;
}

double
string_to_double(const std::string& s)
{
  std::istringstream i(s);
  double x;
  if (!(i >> x))
    return 0;
  return x;
}

