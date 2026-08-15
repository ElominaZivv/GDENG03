using UnityEngine;
using UnityEditor;
using System;
using System.IO;
using System.Collections.Generic;
using UnityEditor.SceneManagement;
using UnityEngine.SceneManagement;
// using Newtonsoft.Json;

[Serializable]
public enum JazzyComponentType
{
    COMP_Cube = 0,
    COMP_Plane,
    COMP_Capsule,
    COMP_Cylinder,
    COMP_Sphere,
    COMP_Mesh,
    COMP_RigidBody
}

[Serializable]
public struct JazzyMaterial
{
    public string shader;
    public string[] textures;
}

[Serializable]
public struct JazzyResources
{
    public JazzyMaterial material;
    public string mesh;
}

[Serializable]
public struct JazzyTransform
{
    public List<float> position;
    public List<float> scale;
    public List<float> rotation;
}

[Serializable]
public struct JazzyGameObject
{
    public string name;
    public string id;
    public string parentId;

    public List<string> components;

    public JazzyResources resources;

    public JazzyTransform transform;
}

[Serializable]
public struct JazzyLevelFormat
{
    public String format;
    public List<JazzyGameObject> objects;
    public int version;
}

public class JazzyLevelParser
{
    [MenuItem("Assets/JAZZY/Import JAZZY level")]
    private static void ImportJazzyLevel()
    {   
        // Get JAZZY Level as JSON
        var selected = Selection.activeObject;
        string jazzyLevelName = Path.ChangeExtension(Path.GetFileName(AssetDatabase.GetAssetPath(selected)), null);
        string jsonText = File.ReadAllText(AssetDatabase.GetAssetPath(selected));
        JazzyLevelFormat level = JsonUtility.FromJson<JazzyLevelFormat>(jsonText);
        // JazzyLevelFormat level = Newtonsoft.Json.JsonConvert.DeserializeObject<JazzyLevelFormat>(jsonText);
        
        // Create a new Unity Scene
        Scene newScene = EditorSceneManager.NewScene(NewSceneSetup.EmptyScene, NewSceneMode.Single);

        // Populate Unity Scene with JAZZY level gameobjects
        for (int i = 0; i < level.objects.Count; i++)
        {
            GameObject gameObject = null;
            switch (level.objects[i].components[0])
            {
                case "Cube":
                    gameObject = GameObject.CreatePrimitive(PrimitiveType.Cube);
                    break;
                case "Plane":
                    gameObject = GameObject.CreatePrimitive(PrimitiveType.Plane);
                    break;
                case "Sphere":
                    gameObject = GameObject.CreatePrimitive(PrimitiveType.Sphere);
                    break;
                case "Cylinder":
                    gameObject = GameObject.CreatePrimitive(PrimitiveType.Cylinder);
                    break;
                case "Capsule":
                    gameObject = GameObject.CreatePrimitive(PrimitiveType.Capsule);
                    break;
            }
            
            if (null != gameObject)
            {
                gameObject.transform.position = new Vector3(level.objects[i].transform.position[0], level.objects[i].transform.position[1], level.objects[i].transform.position[2]);
                if (level.objects[i].components[0] == "Plane") gameObject.transform.localScale = new Vector3(level.objects[i].transform.scale[0]/10.0f, level.objects[i].transform.scale[1]/10.0f, level.objects[i].transform.scale[2]/10.0f);
                else gameObject.transform.localScale = new Vector3(level.objects[i].transform.scale[0], level.objects[i].transform.scale[1], level.objects[i].transform.scale[2]);
                Vector3 rotation = new Vector3(level.objects[i].transform.rotation[0], level.objects[i].transform.rotation[1], level.objects[i].transform.rotation[2]);
                gameObject.transform.rotation = Quaternion.Euler(rotation.x, rotation.y, rotation.z);
            }
            
        }
        string targetDirectory = "Assets/JAZZY_Levels";
        string newSceneDirectory = Path.Combine(targetDirectory, jazzyLevelName);
        string newUnitySceneDirectory = Path.ChangeExtension(newSceneDirectory, ".unity");
        EditorSceneManager.SaveScene(EditorSceneManager.GetActiveScene(), newUnitySceneDirectory);
        AssetDatabase.Refresh();

        Debug.Log($"JAZZY Level Name: {jazzyLevelName}, Format: {level.format}, Version: {level.version}");
    }

    [MenuItem("Assets/JAZZY/Export to JAZZY level")]
    private static void ExportJazzyLevel()
    {
        JazzyLevelFormat jazzyLevel = new JazzyLevelFormat();
        jazzyLevel.format = "jazzy-level";
        jazzyLevel.version = 1;


        string json = JsonUtility.ToJson(jazzyLevel);

        string fullPath = "Assets/JAZZY-Levels";

        File.WriteAllText(fullPath, json);
        Debug.Log($"File saved successfully at: {fullPath}");
    }
}
