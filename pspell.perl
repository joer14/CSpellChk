#!/usr/bin/perl
# $Id: pspell.perl,v 1.2 2012-12-06 13:11:12-08 - - $
use strict;
use warnings;
use Getopt::Std;

$0 =~ s|^(.*/)?([^/]+)/*$|$2|;
my $exit_status = 0;
sub note(@) {print STDERR "$0: @_"}
$SIG{__WARN__} = sub {note @_; $exit_status = 2};
$SIG{__DIE__} = sub {warn @_; exit};
END {exit $exit_status}

my %options;
getopts "nd:", \%options;

my %dictionary;
#my $defdict = "words";
#my $defdict = "/afs/cats.ucsc.edu/courses/cmps012b-wm/usr/dict/words";
my $defdict = "/usr/share/dict/words";
sub load_dictionary($) {
   my ($dictname) = @_;
   open my $dict, "<$dictname" or do {warn "$dictname: $!\n"; return};
   map {chomp; $dictionary{$_} = 1} <$dict>;
   close $dict;
}
load_dictionary $defdict unless $options{'n'};
load_dictionary $options{'d'} if defined $options{'d'};
die "dictionary is empty\n" unless %dictionary;

my $numpat = qr{([[:digit:]]+([-:.][[:digit:]]+)*)};
my $wordpat = qr{([[:alnum:]]+([-&'.][[:alnum:]]+)*)};
for my $filename (@ARGV ? @ARGV : "-") {
   open my $file, "<$filename" or do {warn "$filename: $!\n"; next};
   while (defined (my $line = <$file>)) {
      while ($line =~ s{^.*?($wordpat)}{}) {
         my $word = $1;
         next if $word =~ m{$numpat}
              || $dictionary{$word} || $dictionary{lc $word};
         $exit_status ||= 1;
         print "$filename: $.: $word\n";
      }
   }
   close $file;
}

