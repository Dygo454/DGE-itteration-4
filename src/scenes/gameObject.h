#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include<iostream>
#include"../vec.h"
#include"../quat.h"

class gameObject {
private:
    class component {
    private:
        gameObject parent;
    public:
        virtual void setParent(gameObject p) {parent = p}
        virtual void Start();
        virtual void Update();
        virtual void OnDestroy();
        virtual void OnEnable();
        virtual void OnDisable();
    };
    class Transform : public virtual component {
    private:
        vec3 pos;
        Quat rot;
        vec3 scale(1,1,1);
    public:
        vec3& getPos() {return pos;}
        Quat& getRot() {return rot;}
        vec3& getScale() {return scale;}
    };
    gameObject* parent = NULL;
    std::vector<gameObject*> children;
    std::vector<component*> comps;
    Transform transform;
    std::string name;
    boolean isActive = true;
    std::string layer = "Default";// eventually make its own class
    std::string tag = "NONE";// eventually make its own class
public:
    gameObject(std::string name = "Game Object", vec3 p = vec3(), Quat r = Quat()) {this->name = name;transform.getPos()=p;transform.getRot()=r;comps.push_back(transform);}//open from file
    gameObject(vec3 p, Quat r = Quat()) {this->name = "Game Object";transform.getPos()=p;transform.getRot()=r;comps.push_back(transform);}//open from file
    Transform& getTransform() {return transform;}
    void addComponent(component& c);
    gameObject& removeComponent(int ind);
    template<typename T>
    gameObject& removeComponent(int num = 0);
    template<typename T>
    T* getComponent(int num = 0);
    void setName(std::string n) {name = n;}
    void setActive(boolean a) {isActive = a;}
    void setLayer(string l) {layer = l;}
    void setTag(string t) {tag = t;}
    void setParent(gameObject* p) {parent = p;}
    std::string getName() {return name;}
    boolean getActive() {return isActive;}
    std::string getLayer() {return layer;}
    std::string getTag() {return tag;}
    gameObject* getParent() {return parent;}
    void addChild(gameObject& c);
    gameObject* getChild(int ind = 0);
    gameObject* getChild(std::string n);
    static gameObject* getByTag(string t);
    static gameObject* getByName(string t);
};

template<typename T>
gameObject& removeComponent(int num) {
}
template<typename T>
T* getComponent(int num) {
}

#endif
