// -*- mode: c++; indent-tabs-mode: nil; -*-
//
// Starka
// Copyright (c) 2009-2014 Illumina, Inc.
//
// This software is provided under the terms and conditions of the
// Illumina Open Source Software License 1.
//
// You should have received a copy of the Illumina Open Source
// Software License 1 along with this program. If not, see
// <https://github.com/sequencing/licenses/>
//

///
/// \author Chris Saunders
///

#include "pedicure_info.hh"
#include "pedicure_option_parser.hh"
#include "pedicure_run.hh"
#include "pedicure.hh"

#include "blt_common/blt_arg_parse_util.hh"
#include "blt_util/blt_exception.hh"
#include "blt_util/log.hh"
#include "common/Exceptions.hh"
#include "starling_common/starling_arg_parse.hh"

#include "boost/program_options.hpp"

#include <cassert>
#include <cstdlib>

#include <fstream>
#include <sstream>



namespace
{
const prog_info& pinfo(pedicure_info::get());
}



void
pedicure::
runInternal(int argc,char* argv[]) const
{
    pedicure_options opt;

    for (int i(0); i<argc; ++i)
    {
        if (i) opt.cmdline += ' ';
        opt.cmdline += argv[i];
    }

    // hack in VQSR for pedicure only:
    opt.is_compute_somatic_VQSRmetrics = true;

    std::vector<std::string> legacy_starling_args;
    po::variables_map vm;
    try
    {
        po::options_description visible(get_pedicure_option_parser(opt));
        po::parsed_options parsed(po::command_line_parser(argc,argv).options(visible).allow_unregistered().run());
        po::store(parsed,vm);
        po::notify(vm);

        // allow remaining options to be parsed using starling command-line parser:
        legacy_starling_args = po::collect_unrecognized(parsed.options,po::include_positional);
    }
    catch (const boost::program_options::error& e)
    {
        pinfo.usage(e.what());
    }

    if ((argc==1) || vm.count("help"))
    {
        pinfo.usage();
    }

    // temp workaround for blt/starling options which are not (yet)
    // under program_options control:
    //
    arg_data ad(legacy_starling_args,pinfo,opt.cmdline);
    legacy_starling_arg_parse(ad,opt);

    finalize_pedicure_options(pinfo,vm,opt);

    pedicure_run(pinfo,opt);
}