# Apache License 2.0

import pytest

from ostk.physics.coordinate import Frame

from ostk.simulation import Component
from ostk.simulation import ComponentConfiguration
from ostk.simulation import ComponentHolder
from ostk.simulation import Entity
from ostk.simulation import Satellite


@pytest.fixture
def parent_component() -> Component:
    return Component(
        id="parent-id",
        name="Parent",
        type=Component.Type.Assembly,
        tags=[],
        geometries=[],
        components=[],
        parent=None,
        frame=Frame.GCRF(),
        simulator=None,
    )


@pytest.fixture
def child_component(parent_component: Component) -> Component:
    return Component(
        id="child-id",
        name="Child",
        type=Component.Type.Sensor,
        tags=["optical"],
        geometries=[],
        components=[],
        parent=parent_component,
        frame=Frame.GCRF(),
        simulator=None,
    )


@pytest.fixture
def frameless_component() -> Component:
    return Component(
        id="camera-id",
        name="Camera",
        type=Component.Type.Sensor,
        tags=["a"],
        geometries=[],
        components=[],
        parent=None,
        frame=None,
        simulator=None,
    )


class TestComponent:
    @pytest.mark.parametrize(
        "id, name",
        [
            ("", "Camera"),
            ("camera-id", ""),
            ("", ""),
        ],
    )
    def test_constructor_with_empty_id_or_name(self, id: str, name: str):
        with pytest.raises(RuntimeError):
            Component(
                id=id,
                name=name,
                type=Component.Type.Sensor,
                tags=[],
                geometries=[],
                components=[],
                parent=None,
                frame=Frame.GCRF(),
                simulator=None,
            )

    def test_has_no_undefined(self):
        assert not hasattr(Component, "undefined")
        assert not hasattr(Component, "is_defined")

    def test_without_frame(
        self,
        frameless_component: Component,
        parent_component: Component,
    ):
        assert frameless_component.get_id() == "camera-id"
        assert frameless_component.get_name() == "Camera"
        assert frameless_component.get_type() == Component.Type.Sensor
        assert frameless_component.get_tags() == ["a"]
        assert frameless_component.get_geometries() == []

        with pytest.raises(RuntimeError):
            frameless_component.get_frame()

        parent_component.add_component(frameless_component)

        assert parent_component.has_component_with_name("Camera")

        with pytest.raises(RuntimeError):
            Component.configure(
                ComponentConfiguration(id="lens-id", name="Lens"),
                frameless_component,
            )

    def test_constructor_with_type_none(self):
        component: Component = Component(
            id="component-id",
            name="Component",
            type=None,
            tags=[],
            geometries=[],
            components=[],
            parent=None,
            frame=Frame.GCRF(),
            simulator=None,
        )

        assert component.get_type() is None

    def test_get_type(self, parent_component: Component):
        assert parent_component.get_type() == Component.Type.Assembly

    def test_configure_without_type(self, parent_component: Component):
        for configuration in (
            ComponentConfiguration(id="camera-id", name="Camera"),
            ComponentConfiguration(id="camera-id", name="Camera", type=None),
        ):
            component: Component = Component.configure(configuration, parent_component)

            assert component.get_type() is None

    def test_configure_with_type(self, parent_component: Component):
        component: Component = Component.configure(
            ComponentConfiguration(
                id="camera-id",
                name="Camera",
                type=Component.Type.Sensor,
            ),
            parent_component,
        )

        assert component.get_type() == Component.Type.Sensor

    def test_type_has_no_undefined(self):
        assert not hasattr(Component.Type, "Undefined")
        assert Component.string_from_type(Component.Type.Sensor) == "Sensor"

    def test_entity_methods(self, parent_component: Component):
        assert parent_component.get_id() == "parent-id"
        assert parent_component.get_name() == "Parent"

    def test_add_component_on_python_created_component(
        self,
        parent_component: Component,
        child_component: Component,
    ):
        parent_component.add_component(child_component)

        assert parent_component.has_component_with_id("child-id")
        assert parent_component.has_component_with_name("Child")
        assert parent_component.has_component_at("Child")
        assert parent_component.get_component_with_id("child-id").get_name() == "Child"
        assert parent_component.get_component_with_name("Child").get_id() == "child-id"
        assert parent_component.get_component_at("Child").get_id() == "child-id"
        assert [
            component.get_id() for component in parent_component.get_components()
        ] == ["child-id"]
        assert [
            component.get_id()
            for component in parent_component.get_components_with_tag("optical")
        ] == ["child-id"]

    def test_set_parent(self, parent_component: Component, child_component: Component):
        child_component.set_parent(parent_component)
        child_component.set_parent(None)

    def test_python_base_classes(self):
        # nanobind binds a single base class, so Component has no Python base class and
        # binds the Entity and ComponentHolder methods itself.
        assert issubclass(Satellite, Component)
        assert not issubclass(Component, Entity)
        assert not issubclass(Component, ComponentHolder)

        for name in ("get_id", "get_name"):
            assert hasattr(Entity, name)
            assert hasattr(Component, name)

        for name in (
            "has_component_with_id",
            "has_component_with_name",
            "has_component_at",
            "get_components",
            "get_component_with_id",
            "get_component_with_name",
            "get_components_with_tag",
            "get_component_at",
            "add_component",
        ):
            assert hasattr(ComponentHolder, name)
            assert hasattr(Component, name)

    def test_type_is_a_python_enum(self):
        assert Component.Type.Sensor.name == "Sensor"

        with pytest.raises(TypeError):
            int(Component.Type.Sensor)
