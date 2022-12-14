/****************************************************************************
**  SCALASCA    http://www.scalasca.org/                                   **
**  KOJAK       http://www.fz-juelich.de/jsc/kojak/                        **
*****************************************************************************
**  Copyright (c) 1998-2009                                                **
**  Forschungszentrum Juelich, Juelich Supercomputing Centre               **
**                                                                         **
**  See the file COPYRIGHT in the package base directory for details       **
****************************************************************************/

#ifndef OMPREGION_H
#define OMPREGION_H

#include <iostream>
  using std::ostream;
#include <set>
  using std::set;
#include <string>
  using std::string;

class OMPRegion {
public:
  OMPRegion(const string& n, const string& file, int i, int bfl, int bll,
            bool outr = false);

  static void generate_header(ostream& os);

  void generate_descr(ostream& os);

  void finish();

  string name;
  string file_name;
  string sub_name;
  int    id;
  int    begin_first_line;
  int    begin_last_line;
  int    end_first_line;
  int    end_last_line;
  int    num_sections;
  bool   noWaitAdded;
  bool   outer_reg;
  set<int> descrs;
  OMPRegion* enclosing_reg;

  static OMPRegion* outer_ptr;
};

#endif
