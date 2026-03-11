#pragma once

#ifndef OHTOAI_SUGAR_FACTORY_HH
#define OHTOAI_SUGAR_FACTORY_HH

#include <map>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>

namespace ai::sugar {
    namespace factory {
        // Abstract registrar interface; each concrete product type provides one instance
        template <typename ProductType_t>
        class IProductRegistrar {
        public:
            virtual std::shared_ptr<ProductType_t> createProduct() = 0;
        protected:
            IProductRegistrar() = default;
            virtual ~IProductRegistrar() = default;
            IProductRegistrar(const IProductRegistrar&) = delete;
            IProductRegistrar& operator=(const IProductRegistrar&) = delete;
        };

        // Singleton factory that owns the registry of named product creators
        template <typename ProductType_t>
        class ProductFactory {
        public:
            static ProductFactory<ProductType_t>& instance() {
                static ProductFactory<ProductType_t> instance;
                return instance;
            }

            // Register a product creator under the given name
            void registerProduct(IProductRegistrar<ProductType_t>* registrar, const std::string& name) {
                productRegistry_.emplace(name, registrar);
            }

            // Create and return the product registered under name, or nullptr if not found
            std::shared_ptr<ProductType_t> product(const std::string& name) const {
                auto it = productRegistry_.find(name);
                return it != productRegistry_.end() ? it->second->createProduct() : nullptr;
            }

            bool hasProduct(const std::string& name) const {
                return productRegistry_.find(name) != productRegistry_.end();
            }

            std::vector<std::string> getProductNames() const {
                std::vector<std::string> keys;
                keys.reserve(productRegistry_.size());
                std::transform(
                    productRegistry_.begin(), productRegistry_.end(),
                    std::back_inserter(keys),
                    [](const auto& pair) { return pair.first; });
                return keys;
            }

        protected:
            ProductFactory() = default;
            ~ProductFactory() = default;
            ProductFactory(const ProductFactory&) = delete;
            ProductFactory& operator=(const ProductFactory&) = delete;

            // Registry mapping product name → registrar pointer
            std::map<std::string, IProductRegistrar<ProductType_t>*> productRegistry_;
        };

        // Concrete registrar: self-registers with the factory on construction.
        // ProductImpl_t must be derived from ProductType_t.
        template <typename ProductType_t, typename ProductImpl_t,
            typename = std::enable_if_t<std::is_base_of_v<ProductType_t, ProductImpl_t>>>
        class ProductRegistrar : public IProductRegistrar<ProductType_t> {
        public:
            explicit ProductRegistrar(const std::string& name) {
                ProductFactory<ProductType_t>::instance().registerProduct(this, name);
            }

            std::shared_ptr<ProductType_t> createProduct() override {
                return std::make_shared<ProductImpl_t>();
            }
        };
    } // namespace factory
} // namespace ai::sugar

#endif // !OHTOAI_SUGAR_FACTORY_HH