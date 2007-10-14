#ifndef __MDI_FILELIST_H__
#define __MDI_FILELIST_H__

#include "mdi_c.h"

#include "ui_filelistdialog.h"

class FileListDlg : public MdiChild, private Ui::DialogFileList
{
	Q_OBJECT
public:
	FileListDlg(QWidget *parent);
};

#endif // __MDI_FILELIST_H__