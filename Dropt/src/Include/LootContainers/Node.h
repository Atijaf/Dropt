#pragma once
#include "../Core/CoreLoot.h"

namespace Dropt {
	namespace impl {


		template <typename LootType, Variance Variant>
		class BaseNode : public CoreLootContainer<LootType, Variant>
		{
		public:
			BaseNode(CoreLootContainer<LootType, Variant>& _NodeRef):
			NodeRef(_NodeRef) {}

			bool GetLoot(std::list<LootType*>& OutLoot) override {
				return NodeRef.GetLoot(OutLoot);
			}

		protected:

			bool ShouldRemoveFromContainer() const override {
				return NodeRef.ShouldRemoveFromContainer();
			}


			bool IsFinalized() const override {
				return NodeRef.IsFinalized();
			}

			constexpr Obtainabilities GetObtainability() const override {
				return NodeRef.GetObtainability();
			}

			bool FinalizeLoot() override {
				return NodeRef.FinalizeLoot();
			};

			CoreLootContainer<LootType, Variant>& NodeRef;
		};

		template <typename LootType, Variance Variant>
		class Node : public BaseNode<LootType, Variant>
		{
		public:
			Node(CoreLootContainer<LootType, Variant>& _NodeRef) :
				BaseNode<LootType,Variant>(_NodeRef) {}
		};


		template <typename LootType>
		class Node<LootType, Variance::Chance> : public BaseNode<LootType, Variance::Chance>
		{
		public:
			Node(CoreLootContainer<LootType, Variance::Chance>& _NodeRef) :
				BaseNode<LootType, Variance::Chance>(_NodeRef) 
			{
				this->SetWeight(this->NodeRef.GetWeight());
			}
		};

		template <typename LootType>
		class Node<LootType, Variance::Interval> : public BaseNode<LootType, Variance::Interval>
		{
		public:
			Node(CoreLootContainer<LootType, Variance::Interval>& _NodeRef) :
				BaseNode<LootType, Variance::Interval>(_NodeRef) 
			{
				this->SetInterval(this->NodeRef.GetInterval());
			}
		};

		template <typename LootType>
		class Node<LootType, Variance::Constant> : public BaseNode<LootType, Variance::Constant>
		{
		public:
			Node(CoreLootContainer<LootType, Variance::Constant>& _NodeRef) :
				BaseNode<LootType, Variance::Constant>(_NodeRef) {};
		};

	}
}