

template <typename Key, typename Message, typename Tag>
class MessageFactory {
 public:
  using Creator = std::function<std::unique_ptr<Message>()>;
  using key_type = Key;

  static MessageFactory& getInstance() {
    static MessageFactory instance;
    return instance;
  }

  std::unique_ptr<Message> create(const Key& key) {
    auto it = creators_.find(key);
    if (it == creators_.end()) {
      throw std::runtime_error("No such message type");
    }
    return it->second();
  }

  template <typename T>
  void registerMessage(const Key& key) { 
    creators_[key] = []{ return std::make_unique<T>();}; 
}

  void removeMessage(const Key& key) { creators_.erase(key); }

 private:
  MessageFactory() = default;
  ~MessageFactory() = default;
  MessageFactory(const MessageFactory&) = delete;
  MessageFactory& operator=(const MessageFactory&) = delete;
  std::unordered_map<Key, Creator> creators_;
};

template <typename Factory, typename MsgT>
struct AutoRegister {
    AutoRegister(const typename Factory::key_type& key) {
        Factory::getInstance().template registerMessage<MsgT>(key);
    }
};


#define CONCAT_IMPL(x, y) x##y
#define CONCAT(x, y) CONCAT_IMPL(x, y)

#define REGISTER_MESSAGE(factory, key, type) \
    static AutoRegister<factory, type> CONCAT(reg_##type##_, __LINE__)(key);