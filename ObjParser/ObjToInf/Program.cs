using System;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Windows.Forms;
using ObjParser;

namespace ObjToInf
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            var obj = new Obj();

            var filePath = string.Empty;
            var path = string.Empty;

            using (OpenFileDialog openFileDialog = new OpenFileDialog())
            {
                openFileDialog.Filter = "Wavefront files (*.obj)|*.obj";
                openFileDialog.FilterIndex = 2;
                openFileDialog.RestoreDirectory = true;

                if (openFileDialog.ShowDialog() == DialogResult.OK)
                {
                    filePath = openFileDialog.FileName;
                    path = Path.GetDirectoryName(filePath);
                    var fileStream = openFileDialog.OpenFile();
                }
            }

            obj.LoadObj(filePath);

            FileStream file = new FileStream(path + "\\Vertex.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.None);
            file.Close();
            StreamWriter writeFile = new StreamWriter(path + "\\Vertex.txt");
            for (int i = 0; i < obj.VertexList.Count(); i++)
                writeFile.WriteLine(obj.VertexList[i]);
            writeFile.Close();

            if (obj.NormalList.Count > 0)
            {
                file = new FileStream(path + "\\Normal.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.None);
                file.Close();
                writeFile = new StreamWriter(path + "\\Normal.txt");
                for (int i = 0; i < obj.NormalList.Count(); i++)
                    writeFile.WriteLine(obj.NormalList[i]);
                writeFile.Close();
            }

            file = new FileStream(path + "\\Texture.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.None);
            file.Close();
            writeFile = new StreamWriter(path + "\\Texture.txt");
            for (int i = 0; i < obj.TextureList.Count(); i++)
                writeFile.WriteLine(obj.TextureList[i]);
            writeFile.Close();

            file = new FileStream(path + "\\Faces.txt", FileMode.OpenOrCreate, FileAccess.ReadWrite, FileShare.None);
            file.Close();
            writeFile = new StreamWriter(path + "\\Faces.txt");
            for (int i = 0; i < obj.FaceList.Count(); i++)
                writeFile.WriteLine(obj.FaceList[i]);
            writeFile.Close();

            Process.Start("obj2inf.exe");
        }
    }
}
