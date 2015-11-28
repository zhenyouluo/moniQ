#-------------------------------------------------
#
# Project created by QtCreator 2015-09-26T12:45:28
#
#-------------------------------------------------

TEMPLATE = subdirs

SUBDIRS += main.pro \
    moniQscheduling \
    moniQanalysis

win32
{
  SUBDIRS +=pingwin
}

SUBDIRS -=pingwin

