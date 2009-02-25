#!/usr/bin/perl

use File::Find;
use File::Copy;
use File::Basename;
use POSIX qw(strftime);

$deploydir="../../deploy/win32/";

$incdir="$deploydir/include/rise";
$libdir="$deploydir/lib";

mkdir "$deploydir" || die "can't mkdir($deploydir) : $!";;

# �����㥬 ������
mkdir "$deploydir/include" || die "can't mkdir($deploydir/include) : $!";;
mkdir "$incdir" || die "can't mkdir($incdir) : $!";;

#������� ᯨ᮪ 䠩��� ��⠫���
sub GetTargetFileList
{
  my $CurrDir        =  shift; # ⥪�騩 ��⠫��
  my $TargetFileList =  shift; # ᯨ᮪ 䠩��� 楫�

  # �������� �᫨ �� ��।����� ��ࠬ����
  return undef unless (defined($CurrDir));
  return undef unless (defined($TargetFileList));

  my $file;
  
  local *DIR;

  my (@all);

  opendir(DIR,$CurrDir) || die "�訡�� �⥭�� ��⠫��� $CurrDir: $!\n";

  @all = readdir(DIR);

  # 横� �� �ᥬ ����⠬ ��⠫���
  foreach $file (@all)
  {
    next if (($file eq ".") || ($file eq ".."));

    if (-d "$CurrDir\/$file")
    {
      # ��������� ��⠫���
      GetTargetFileList("$CurrDir\/$file",$TargetFileList);
    }
    else
    {
      # ��������� 䠩��
      # �஢�ઠ 䠩�� �� 蠡���
      if ($file =~ m/^*.(h|hpp)$/x)
      {
        push(@{$TargetFileList},"$CurrDir\/$file");
      }
    }
  }
  closedir(DIR);
}


#���᮪ 䠩��� ��⠫���    
my $FileList = [];
my $file;

GetTargetFileList("../../rise", $FileList);

foreach $file (@{$FileList})
{
  local $outfile = $file;
  $outfile =~ s/^rise//gx;
  $outfile = "$incdir$outfile";

  local $deploydircur = dirname("$outfile");

  if( ! -d "$deploydircur" )
  {
    mkdir("$deploydircur") || die "can't mkdir($deploydircur) : $!";
  }
  print "$file -> $outfile\n";
  copy("$file", "$outfile") || die "can't copy file '$file -> $outfile': $!";
}

# �����㥬 ����
mkdir "$libdir" || die "can't mkdir($libdir) : $!";
copy("Debug/rise.dll", "$libdir") || die "can't copy file '$file -> $outfile': $!";
copy("Debug/rise.lib", "$libdir") || die "can't copy file '$file -> $outfile': $!";

copy("../version.txt", "$deploydir/");

