#ifndef __PLUGIN_LOADER_H__
#define __PLUGIN_LOADER_H__

class PluginLoader : public QObject {
public:
    static PluginInterface *forExtension(const QString& extension);
    static PluginInterface *forMimeType(const QString& mimeType);
    static PluginInterface *forName(const QString& name);
    //static ??? supportedExtensions();
    //static ??? supportedMimeTypes();
private:
    PluginLoader();
    ~PluginLoader();
};

#endif
