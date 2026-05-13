class Entity
{
public:
    virtual ~Entity() = default;
    virtual void Start();
    virtual void Update(float deltaTime);
};