/*
 * Codonphaser.hh
 *
 *  Test for codon-phasing.
 *
 *  Created on: Aug 10, 2013
 *  Author: Morten Kallberg
 */

#ifndef CODONPHASER_HH_
#define CODONPHASER_HH_

#include "starling_common/gvcf_locus_info.hh"
#include "starling_common/starling_read_buffer.hh"

class Codon_phaser {
public:
    Codon_phaser();
    virtual ~Codon_phaser(){};

public:
    bool add_site(site_info& si);       // add site to buffer
    void clear_buffer(void);            // clear site buffer
    void make_record(void);             // make phased record
    void write_out_buffer();            // debugging feature, print current buffer to std
    void write_out_alleles();           // print allele evidence
    void clear_read_buffer(int pos);    // free up read that are no longer in phasing evidence, up to and including this position
    void collect_read_evidence();       // fill in allele counter
    void construct_reference();         // assemble the reference allele for the record
    void create_phased_record();        // fill in the si record and decide if we have sufficient evidence for a phased call
    bool is_in_block;                   // Are we currently in a phasing block
    std::vector<site_info> buffer;      // buffer of het snp calls
    starling_read_buffer *read_buffer;  // pass along the relevant read-buffer
private:
    int block_start;                    // position of first added het site to block
    int block_end;                      // position of last added het site to block
    int range;                          // phasing window considered
    int het_count;                      // total hets observed in buffer
    int read_len;                       // the length of the input reads
    int previous_clear;                 // cleared buffer up to this site
    int total_reads;                    // total used reads spanning phasing region
    int total_reads_unused;             // total unused reads spanning phasing region
    int min_baseq;                      // minimum baseq to consider
    int min_mapq;                       // minimum mapq to consider
    bool phase_indels;                  // should we attempt to phase indels as well, if false simply break the block at this point
    std::string reference;              // the phased allele reference
    typedef std::map<std::string,int> allele_map;
    allele_map observations;
    std::string max_alleles[2];         // maintain two max alleles
};
#endif /* CODONPHASER_HH_ */
