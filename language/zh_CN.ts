<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>GSCV_import_wizard</name>
    <message>
        <source>Next &gt;</source>
        <translation type="vanished">下一步 &gt;</translation>
    </message>
    <message>
        <source>&lt; Back</source>
        <translation type="vanished">&lt; 上一步</translation>
    </message>
    <message>
        <source>Cancel</source>
        <translation type="vanished">取消</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="28"/>
        <location filename="../gscv_import_wizard.cpp" line="40"/>
        <source>Warning</source>
        <translation>警告</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="28"/>
        <location filename="../gscv_import_wizard.cpp" line="40"/>
        <source>Packing thread is running. Aborting packing thread can be dangerous. Do you want to continue?</source>
        <translation>正在打包中，强行中断可能产生意想不到的后果，仍然要退出吗？</translation>
    </message>
</context>
<context>
    <name>GSCV_import_wizard_page_1</name>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="54"/>
        <source>GSCV import wizard</source>
        <translation>GSCV导入向导</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="57"/>
        <source>Welcome to GSCV import wizard. This wizard will help you convert a Seurat object (of R language) to a gscv archive.</source>
        <translation>欢迎使用GSCV导入向导。此向导将帮助你将（R语言中的）Seurat对象转换为GSCV归档文件。</translation>
    </message>
</context>
<context>
    <name>GSCV_import_wizard_page_2</name>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="63"/>
        <source>Export seurat object</source>
        <translation>导出Seurat对象</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="64"/>
        <source>Please copy the scripts and paste them in your R console, which will generate intermediate files automatically.</source>
        <translation>请复制这段代码，并粘贴于R控制台中。此函数将自动生成中间文件。</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="67"/>
        <source>This function converts a seurat object to text files. The first argument is a seurat object, and the second argument is the filename of output.</source>
        <translation>此函数将Seurat对象转化为文本文件，第一个参数是Seurat对象，第二个参数是输出文件的文件名。</translation>
    </message>
</context>
<context>
    <name>GSCV_import_wizard_page_3</name>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="93"/>
        <source>Export seurat object</source>
        <translation>导出Seurat对象</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="94"/>
        <source>Please copy the scripts and paste them in your R console, which will generate intermediate files automatically.</source>
        <translation>请复制这段代码，并粘贴于R控制台中。此函数将自动生成中间文件。</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="98"/>
        <source>For example, to process the seurat object &quot;hsa&quot;, use the following script:</source>
        <translation>比如，要处理一个名为“hsa”的Seurat对象，可以使用以下代码：</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="107"/>
        <source>This script exports the object &quot;hsa&quot;, and create a directory called &quot;hsa_2022_7_1&quot;, where the exported text files are saved.</source>
        <translation>这段代码将对象“hsa”导出，并创建一个名为“hsa_2022_7_1”的目录，导出的文件即存放在该目录下。</translation>
    </message>
</context>
<context>
    <name>GSCV_import_wizard_page_4</name>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="117"/>
        <source>Select the directory just created</source>
        <translation>选择刚刚创建的目录</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="118"/>
        <source>Please select the directory just created by the R script.</source>
        <translation>请选择刚刚用R代码创建的目录。</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="122"/>
        <source>The R script creates a directory, whose name is specified by the second argument of export_gscv(). The directory contains four tab-separated values files (*.tsv). Please select this directory by clicking the &quot;select&quot; button.</source>
        <translation>R代码创建了一个目录，其名字是通过函数export_gscv()的第二个参数指定的。此目录包含4个制表位分隔的表格文件（*.tsv），请点击“浏览”按钮并选择这个目录。</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="126"/>
        <source>Directory:</source>
        <oldsource>Directory: </oldsource>
        <translation>目录名：</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="132"/>
        <source>select</source>
        <translation>浏览</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="147"/>
        <source>Choose a directory</source>
        <translation>选择目录</translation>
    </message>
</context>
<context>
    <name>GSCV_import_wizard_page_5</name>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="152"/>
        <location filename="../gscv_import_wizard.cpp" line="182"/>
        <source>Set filename</source>
        <oldsource>Specify filename</oldsource>
        <translation>选择文件名</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="153"/>
        <source>Please set the filename of the output.</source>
        <oldsource>Please specify the filename of the output.</oldsource>
        <translation>请选择输出文件的文件名。</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="157"/>
        <source>The output will be written to a specified file. Please set filename by clicking the &quot;select&quot; button.</source>
        <translation>转换结果将被保存到指定的路径，请点击“浏览”按钮选择保存的文件名。</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="161"/>
        <source>Filename:</source>
        <oldsource>Filename: </oldsource>
        <translation>文件名：</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="167"/>
        <source>select</source>
        <translation>浏览</translation>
    </message>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="183"/>
        <source>GSCV Archive (*.gscvArchive)</source>
        <translation>GSCV归档文件 (*.gscvArchive)</translation>
    </message>
</context>
<context>
    <name>GSCV_import_wizard_page_6</name>
    <message>
        <location filename="../gscv_import_wizard.cpp" line="223"/>
        <source>Progress</source>
        <translation>进度</translation>
    </message>
</context>
<context>
    <name>MainWindow</name>
    <message>
        <location filename="../mainwindow.ui" line="14"/>
        <source>Graphical Single Cell Viewer</source>
        <translation>图形化单细胞查看器</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="100"/>
        <source>File</source>
        <translation>文件</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="107"/>
        <source>Help</source>
        <translation>帮助</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="114"/>
        <source>Visualize</source>
        <translation>可视化</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="118"/>
        <location filename="../mainwindow.cpp" line="143"/>
        <source>Save viewport</source>
        <translation>保存视口</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="131"/>
        <source>Settings</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="143"/>
        <source>Open</source>
        <translation>打开</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="148"/>
        <source>Report bug</source>
        <translation>漏洞报告</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="153"/>
        <location filename="../mainwindow.cpp" line="105"/>
        <source>Cell color</source>
        <translation>细胞颜色</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="158"/>
        <location filename="../mainwindow.cpp" line="119"/>
        <source>Color scheme</source>
        <translation>配色方案</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="163"/>
        <location filename="../mainwindow.cpp" line="133"/>
        <source>Camera settings</source>
        <translation>相机设置</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="168"/>
        <source>Import</source>
        <translation>导入</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="173"/>
        <source>Save as image</source>
        <translation>保存为图片</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="178"/>
        <source>Copy to clipboard</source>
        <translation>复制到剪贴板</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="183"/>
        <source>Settings...</source>
        <translation>设置...</translation>
    </message>
    <message>
        <location filename="../mainwindow.ui" line="188"/>
        <source>View help</source>
        <translation>查看帮助</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="38"/>
        <source>Statistic</source>
        <translation>数据</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="54"/>
        <source>Select a data set</source>
        <translation>请选择一个数据集</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="57"/>
        <source>GSCV Archive (*.gscvArchive)</source>
        <translation>GSCV归档文件 (*.gscvArchive)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="81"/>
        <source>Bug report</source>
        <translation>漏洞报告</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="81"/>
        <source>For bug reporting, please email your complaints to Shi Chengge&lt;1826930551@qq.com&gt;</source>
        <translation>若需要报告漏洞，请发送电子邮件至Shi Chengge&lt;1826930551@qq.com&gt;</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="144"/>
        <source>JPEG (*.jpg);;BMP (*.bmp);;PNG (*.png)</source>
        <translation>JPEG图片 (*.jpg);;BMP图片 (*.bmp);;PNG图片 (*.png)</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="201"/>
        <source>Error</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="../mainwindow.cpp" line="201"/>
        <source>Failed to open documents in browser. You can find documents in directory &quot;doc&quot; in the installation directory.</source>
        <translation>无法在浏览器中打开帮助文档，您可以在安装目录下的“doc”目录中找到帮助文档。</translation>
    </message>
    <message>
        <source>Failed to open browser. You can find documents in directory &quot;doc&quot; in the installation directory.</source>
        <translation type="vanished">无法打开浏览器，您可以</translation>
    </message>
</context>
<context>
    <name>camera_setting</name>
    <message>
        <location filename="../camera_setting.ui" line="14"/>
        <source>Form</source>
        <translation>窗体</translation>
    </message>
    <message>
        <location filename="../camera_setting.ui" line="20"/>
        <source>Scale</source>
        <translation>缩放</translation>
    </message>
    <message>
        <location filename="../camera_setting.ui" line="54"/>
        <source>Point size</source>
        <translation>点的大小</translation>
    </message>
    <message>
        <location filename="../camera_setting.ui" line="101"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
</context>
<context>
    <name>cell_color</name>
    <message>
        <location filename="../cell_color.ui" line="14"/>
        <source>Form</source>
        <translation>窗体</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="26"/>
        <source>Expression</source>
        <translation>基因表达量</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="40"/>
        <source>Search gene</source>
        <translation>按基因名搜索</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="56"/>
        <location filename="../cell_color.ui" line="104"/>
        <source>Go</source>
        <translation>搜索</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="74"/>
        <source>Meta data</source>
        <translation>元数据</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="88"/>
        <source>Meta</source>
        <translation>元数据类型</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="128"/>
        <source>Continous</source>
        <translation>连续变量</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="135"/>
        <source>Categorical</source>
        <translation>离散变量</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="144"/>
        <source>Preview</source>
        <translation>预览</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="157"/>
        <source>Clear</source>
        <translation>清除颜色</translation>
    </message>
    <message>
        <location filename="../cell_color.ui" line="177"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
    <message>
        <location filename="../cell_color.cpp" line="30"/>
        <source>Information</source>
        <translation>提示</translation>
    </message>
    <message>
        <location filename="../cell_color.cpp" line="30"/>
        <source>Failed to find </source>
        <translation>找不到 </translation>
    </message>
    <message>
        <location filename="../cell_color.cpp" line="30"/>
        <source>.</source>
        <translation>。</translation>
    </message>
    <message>
        <location filename="../cell_color.cpp" line="41"/>
        <source>Critical</source>
        <translation>错误</translation>
    </message>
    <message>
        <location filename="../cell_color.cpp" line="41"/>
        <source>Internal error.</source>
        <translation>内部错误。</translation>
    </message>
</context>
<context>
    <name>color_scheme</name>
    <message>
        <location filename="../color_scheme.ui" line="20"/>
        <source>Form</source>
        <translation>窗体</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="32"/>
        <source>Gradient</source>
        <translation>渐变</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="40"/>
        <source>Bicolored</source>
        <translation>双色</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="47"/>
        <source>Tricolored</source>
        <translation>三色</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="77"/>
        <source>High</source>
        <translation>高</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="114"/>
        <source>Middle</source>
        <translation>中</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="151"/>
        <source>Low</source>
        <translation>低</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="183"/>
        <source>Discrete</source>
        <translation>离散</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="201"/>
        <source>Add</source>
        <translation>增加</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="208"/>
        <source>Remove</source>
        <translation>删除</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="215"/>
        <source>Up</source>
        <translation>上移</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="222"/>
        <source>Down</source>
        <translation>下移</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="247"/>
        <source>Other</source>
        <translation>其他</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="261"/>
        <source>Inactive</source>
        <translation>非活动细胞</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="298"/>
        <source>Background</source>
        <translation>背景</translation>
    </message>
    <message>
        <location filename="../color_scheme.ui" line="343"/>
        <source>Close</source>
        <translation>关闭</translation>
    </message>
</context>
<context>
    <name>sc_packer</name>
    <message>
        <location filename="../sc_packer.cpp" line="25"/>
        <source>An error occurred when checking files.
Exit.</source>
        <translation>在检查文件时发生错误。
退出。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="29"/>
        <source>An error occurred when packing files.
Exit.</source>
        <translation>在打包文件时发生错误。
退出。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="32"/>
        <source>The files are successfully packed.</source>
        <translation>已将文件成功打包。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="40"/>
        <source>Packing </source>
        <translation>正在打包 </translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="40"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="49"/>
        <source>Packing index...</source>
        <translation>正在打包索引...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="51"/>
        <location filename="../sc_packer.cpp" line="68"/>
        <location filename="../sc_packer.cpp" line="85"/>
        <location filename="../sc_packer.cpp" line="102"/>
        <location filename="../sc_packer.cpp" line="606"/>
        <location filename="../sc_packer.cpp" line="616"/>
        <location filename="../sc_packer.cpp" line="625"/>
        <location filename="../sc_packer.cpp" line="636"/>
        <source>    Failed.</source>
        <translation>    失败。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="52"/>
        <location filename="../sc_packer.cpp" line="69"/>
        <location filename="../sc_packer.cpp" line="86"/>
        <location filename="../sc_packer.cpp" line="103"/>
        <location filename="../sc_packer.cpp" line="607"/>
        <location filename="../sc_packer.cpp" line="617"/>
        <location filename="../sc_packer.cpp" line="626"/>
        <location filename="../sc_packer.cpp" line="637"/>
        <source>Stop.</source>
        <translation>停止。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="66"/>
        <source>Packing meta data...</source>
        <translation>正在打包元数据...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="83"/>
        <source>Packing position...</source>
        <translation>正在打包位置信息...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="100"/>
        <source>Packing expression data...</source>
        <translation>正在打包表达数据...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="127"/>
        <source>Cleaning up...</source>
        <translation>清理中...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="205"/>
        <source>Failed to find cell index file.</source>
        <translation>找不到索引文件。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="230"/>
        <source>Failed to find cell position file.</source>
        <translation>找不到位置文件。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="246"/>
        <location filename="../sc_packer.cpp" line="268"/>
        <location filename="../sc_packer.cpp" line="505"/>
        <source>Cell names in cell index don&apos;t match cell names in cell position file.</source>
        <translation>索引文件中的细胞名与位置文件中的细胞名不匹配。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="257"/>
        <location filename="../sc_packer.cpp" line="279"/>
        <source>Wrong column number in cell position file.</source>
        <translation>位置文件中列的数量错误。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="285"/>
        <location filename="../sc_packer.cpp" line="343"/>
        <source>Wrong cell number in cell position file.</source>
        <translation>位置文件中细胞的数量错误。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="295"/>
        <source>Failed to find cell meta file.</source>
        <translation>找不到元数据文件。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="314"/>
        <location filename="../sc_packer.cpp" line="436"/>
        <source>Cell names in cell index don&apos;t match cell names in meta data file.</source>
        <translation>索引文件中的细胞名与元数据文件中的细胞名不匹配。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="353"/>
        <source>Failed to find expression data file.</source>
        <translation>找不到表达数据文件。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="371"/>
        <source>Wrong column number in expression data file.</source>
        <translation>表达数据文件中列的数量错误。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="387"/>
        <source>Failed to open cell index file.</source>
        <translation>无法打开索引文件。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="541"/>
        <source>    Packing </source>
        <translation>    正在打包 </translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="573"/>
        <source>Catenating files...</source>
        <translation>合并文件...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="580"/>
        <source>Cannot open output file.</source>
        <translation>无法打开输出文件。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="604"/>
        <source>Checking index...</source>
        <translation>正在检查索引文件...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="610"/>
        <location filename="../sc_packer.cpp" line="629"/>
        <location filename="../sc_packer.cpp" line="640"/>
        <source>    Find </source>
        <translation>    已找到 </translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="610"/>
        <source> cells.</source>
        <translation> 个细胞。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="611"/>
        <location filename="../sc_packer.cpp" line="620"/>
        <location filename="../sc_packer.cpp" line="631"/>
        <location filename="../sc_packer.cpp" line="641"/>
        <source>    Pass.</source>
        <translation>    通过。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="614"/>
        <source>Checking position...</source>
        <translation>正在检查位置文件...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="623"/>
        <source>Checking meta data...</source>
        <translation>正在检查元数据文件...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="630"/>
        <source> meta data.</source>
        <translation> 条元数据。</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="634"/>
        <source>Checking expression data...</source>
        <translation>正在检查表达数据文件...</translation>
    </message>
    <message>
        <location filename="../sc_packer.cpp" line="640"/>
        <source> genes.</source>
        <translation> 个基因。</translation>
    </message>
</context>
<context>
    <name>settings_dialog</name>
    <message>
        <location filename="../settings_dialog.ui" line="14"/>
        <source>Settings</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="../settings_dialog.ui" line="24"/>
        <source>General</source>
        <translation>通用</translation>
    </message>
    <message>
        <location filename="../settings_dialog.ui" line="32"/>
        <source>Language</source>
        <translation>语言</translation>
    </message>
    <message>
        <location filename="../settings_dialog.ui" line="65"/>
        <source>Performance</source>
        <translation>性能</translation>
    </message>
    <message>
        <location filename="../settings_dialog.ui" line="71"/>
        <source>Repaint instantly</source>
        <translation>立即重绘</translation>
    </message>
    <message>
        <location filename="../settings_dialog.ui" line="78"/>
        <source>Update viewport once painting parameters modified. This might increase GPU usage significantly.</source>
        <oldsource>Update OpenGL widget once painting parameters modified. This might increase GPU usage significantly.</oldsource>
        <translation>当绘图参数改变时，立即更新视口。这可能显著增加GPU使用率。</translation>
    </message>
    <message>
        <location filename="../settings_dialog.ui" line="97"/>
        <source>Viewport maximum frame rate</source>
        <translation>视口最大帧率</translation>
    </message>
    <message>
        <location filename="../settings_dialog.cpp" line="33"/>
        <source>Information</source>
        <translation>提示</translation>
    </message>
    <message>
        <location filename="../settings_dialog.cpp" line="33"/>
        <source>Preferred language is changed. Please restart GSCV to apply the new language.</source>
        <translation>语言偏好已改变，请重启GSCV以应用新语言。</translation>
    </message>
</context>
<context>
    <name>statistic</name>
    <message>
        <location filename="../statistic.ui" line="14"/>
        <source>Form</source>
        <translation>窗体</translation>
    </message>
    <message>
        <location filename="../statistic.cpp" line="17"/>
        <location filename="../statistic.cpp" line="37"/>
        <source>Property</source>
        <translation>属性</translation>
    </message>
    <message>
        <location filename="../statistic.cpp" line="17"/>
        <location filename="../statistic.cpp" line="37"/>
        <source>Value</source>
        <translation>值</translation>
    </message>
    <message>
        <location filename="../statistic.cpp" line="34"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="../statistic.cpp" line="35"/>
        <source>Cells</source>
        <translation>细胞数量</translation>
    </message>
    <message>
        <location filename="../statistic.cpp" line="36"/>
        <source>Genes</source>
        <translation>基因数量</translation>
    </message>
</context>
</TS>
